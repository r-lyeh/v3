/*
tmixer - tiny audio mixer

This is a C-language rewrite of the original C++ library tinymixer
(https://github.com/mendsley/tinymixer), originally authored by
Mathew Endsley, licensed under the BSD 2-Clause License.

All credit goes to the original author.

-------------------------------------------------------------------------------

Original copyright:

Copyright (c) 2011-2013, Mathew Endsley
All rights reserved.

C rewrite and modifications:

Copyright (c) 2025, Arne König
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef TINY_MIXER_H
#define TINY_MIXER_H

#include <stdint.h>

typedef struct tm_buffer tm_buffer;

typedef struct tm_channel {
    int index;
} tm_channel;

typedef struct tm_callbacks {
    void* udata;
    void* (*allocate)(void* opaque, int bytes);
    void  (*free)(void* opaque, void* pointer);
    void (*pre_effects)(void* opaque, float* samples, int nsamples, float gain);
    void (*channel_complete)(void* opaque, void* channel_opaque, tm_channel channel);
} tm_callbacks;

typedef struct tm_buffer_callbacks {
    void (*on_destroy)(void* buffer_opqaue);
    void* (*start_source)(void* buffer_opaque);
    void (*end_source)(void* buffer_opaque, void* source_opaque);
    int (*request_samples)(void* buffer_opaque, void* source_opaque, const float** left, const float** right, int nsamples);
} tm_buffer_callbacks;

typedef struct tm_resampler {
    float ideal_rate;
    float prev_samples[2];
} tm_resampler;

typedef struct tm_lowpass_filter {
    float cutoff_frequency; // 1250.0 is a decent starting point
    float sample_rate;
    float channel_history[2];
} tm_lowpass_filter;

void tm_init(tm_callbacks callbacks, int sample_rate);
void tm_shutdown(void);
void tm_getsamples(float* samples, int nsamples);
void tm_set_mastergain(float gain);

void tm_create_buffer_interleaved_s16le(int channels, const int16_t* pcm_data, int pcm_data_size, const tm_buffer** handle);
void tm_create_buffer_interleaved_float(int channels, const float* pcm_data, int pcm_data_size, const tm_buffer** handle);
void tm_create_buffer_vorbis_stream(const void* data, int ndata, void* opaque, void (*closed)(void*), const tm_buffer** handle);
void tm_create_buffer_custom_stream(void* opaque, tm_buffer_callbacks callbacks, const tm_buffer** buffer);
int tm_get_buffer_size(const tm_buffer* handle);
void tm_release_buffer(const tm_buffer* handle);

void tm_update_listener(const float* position);
void tm_set_base_gain(int index, float gain);
void tm_set_callback_gain(float gain);
void tm_effects_compressor(const float thresholds[2], const float multipliers[2], float attack_seconds, float release_seconds);

bool tm_add(const tm_buffer* handle, int gain_index, float gain, float pitch, tm_channel* channel);
bool tm_add_loop(const tm_buffer* handle, int gain_index, float gain, float pitch, tm_channel* channel);
bool tm_add_spatial(const tm_buffer* handle, int gain_index, float gain, float pitch, const float* position, float distance_min, float distance_max, tm_channel* channel);
bool tm_add_spatial_loop(const tm_buffer* handle, int gain_index, float gain, float pitch, const float* position, float distance_min, float distance_max, tm_channel* channel);

void tm_channel_set_opaque(tm_channel channel, void* opaque);
void tm_channel_stop(tm_channel channel);
void tm_channel_set_position(tm_channel channel, const float* position);
void tm_channel_fadeout(tm_channel channel, float seconds);
void tm_channel_set_gain(tm_channel channel, float gain);
void tm_channel_set_frequency(tm_channel channel, float frequency);

float tm_channel_get_gain(tm_channel channel);
void tm_stop_all_sources();

static inline bool tm_channel_isvalid(tm_channel channel) { return channel.index != 0; }
static inline bool tm_channel_equals(tm_channel lhs, tm_channel rhs) { return lhs.index == rhs.index; }

void tm_resampler_init(tm_resampler* resampler, int input_sample_rate, int output_sample_rate);
void tm_resampler_init_rate(tm_resampler* resampler, float ideal_rate);
int tm_resampler_calculate_input_samples(const tm_resampler* resampler, int output_samples);
int tm_resampler_calculate_output_samples(const tm_resampler* resampler, int input_samples);
void tm_resample_stereo(tm_resampler* resampler, const float* input, int num_input_samples, float* output, int num_output_samples);
void tm_resample_mono(tm_resampler* resampler, const float* input, int num_input_samples, float* output, int num_output_samples);

void tm_lowpass_filter_init(tm_lowpass_filter* filter, float cutoff_frequency, float sample_rate);
void tm_lowpass_filter_apply(tm_lowpass_filter* filter, float* output, float* input, int num_samples, int num_channels);

#endif //TINY_MIXER_H

#ifdef TMIXER_IMPL

//--IMPL--------------------------------------------------------------------------------------------------

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

enum {
    TM_SOURCEFLAG_PLAYING = 1 << 0,
    TM_SOURCEFLAG_POSITIONAL = 1 << 1,
    TM_SOURCEFLAG_LOOPING = 1 << 2,
    TM_SOURCEFLAG_FADEOUT = 1 << 3,
    TM_SOURCEFLAG_FREQUENCY = 1 << 4,
};

typedef struct buffer_t buffer_t;
typedef struct source_t source_t;

typedef struct buffer_functions_t {
    void (*on_destroy)(buffer_t* buffer);
    void (*start_source)(source_t* source);
    void (*end_source)(source_t* source);
    int (*request_samples)(source_t* source, const float** left, const float** right, int nsamples);
    int (*get_buffer_size)(const buffer_t* buffer);
} buffer_functions_t;

typedef struct buffer_t {
    const buffer_functions_t* funcs;
    mt_atomic_int32 refcnt;
} buffer_t;

typedef struct static_source_data_t {
    int32_t sample_pos;
} static_source_data_t;

typedef struct vorbis_stream_data_t {
    stb_vorbis* v;
    const float* outputs[2];
    int noutputs;
} vorbis_stream_data_t;

typedef struct custom_stream_data_t {
    void* opaque;
} custom_stream_data_t;

typedef struct source_t {
    const buffer_t* buffer;
    union {
        static_source_data_t static_source;
        vorbis_stream_data_t vorbis_stream;
        custom_stream_data_t custom_stream;
    } instance_data;
    float position[3];
    float fadeout_per_sample;
    float gain_base;
    float distance_min;
    float distance_difference;
    float frequency;
    uint16_t frame_age;
    uint8_t flags;
    uint8_t gain_base_index;
    tm_resampler resampler;
    void* opaque;
} source_t;

#define N_GAINTYPES 8
#define N_SOURCES 32
#define N_SAMPLES 2048
#define N_SAMPLESF (float)N_SAMPLES
#define SPEAKER_DIST 0.17677669529663688110021109052621f // 1/(4 *sqrtf(2))

static struct {
    mt_mutex lock;
    tm_callbacks callbacks;
    float position[3];
    float gain_master;
    float gain_base[N_GAINTYPES];
    float gain_callback;
    int32_t sample_rate;
    float compressor_last_samples[2];
    float compressor_thresholds[2];
    float compressor_multipliers[2];
    float compressor_factor;
    float compressor_attack_per1ksamples;
    float compressor_release_per1ksamples;
    int32_t samples_remaining;
    source_t sources[N_SOURCES];
    float buffer[2*N_SAMPLES];
    float scratch[2*N_SAMPLES];
} tm;


static inline float _tm_clamp(float v, float min, float max) { return min > v ? min : (v > max ? max : v); }
static inline int _tm_min(int a, int b) { return (a < b) ? a : b; }
static inline float _tm_dist(const float* a, const float* b) {
    const float distsq = (a[0] - b[0])*(a[0] - b[0]) + (a[1] - b[1])*(a[1] - b[1]) + (a[2] - b[2])*(a[2] - b[2]);
    return sqrtf(distsq);
}
static inline void _tm_vcopy(float* v, const float* a) { v[0] = a[0], v[1] = a[1], v[2] = a[2]; }

static void _tm_addref(buffer_t* buffer) {
    ++buffer->refcnt;
}

static void _tm_decref(buffer_t* buffer) {
    if (1 == mt_atomic_decrement(&buffer->refcnt)) {
        buffer->funcs->on_destroy(buffer);
        tm.callbacks.free(tm.callbacks.udata, buffer);
    }
}

static void kill_source(source_t* source) {

    if (tm.callbacks.channel_complete) {
        tm_channel channel;
        channel.index = (int)(source - tm.sources) + 1;

        // unlock the mutex while in the user callback
        mt_mutex_unlock(&tm.lock);
        tm.callbacks.channel_complete(tm.callbacks.udata, source->opaque, channel);
        mt_mutex_lock(&tm.lock);
    }

    if (source->buffer) {
        source->buffer->funcs->end_source(source);
        _tm_decref((buffer_t*)source->buffer);
    }

    source->buffer = 0;
    source->flags = 0;
}

static source_t* find_source(void) {
    source_t* best_source = NULL;
    uint16_t best_age = 0;

    for (int ii = 0; ii < N_SOURCES; ++ii) {
        source_t* source = &tm.sources[ii];
        if (!source->buffer)
            return source;

        if (0 == (source->flags & TM_SOURCEFLAG_LOOPING)) {
            const uint16_t age = source->frame_age;
            if (age >= best_age) {
                best_source = source;
                best_age = age;
            }
        }
    }

    if (NULL != best_source) {
        if (best_source->buffer) {
            kill_source(best_source);
        }
    }
    return best_source;
}

static void render(source_t* source, float* buffer, const float gain[2]) {

    float* left = buffer;
    float* right = buffer + N_SAMPLES;

    int remaining = N_SAMPLES;
    while (remaining > 0) {
        int samples_read = remaining;
        int samples_written = samples_read;

        const float* srcleft;
        const float* srcright;

        // source has a non-1.0f frequency shift
        if (source->flags & TM_SOURCEFLAG_FREQUENCY) {
            samples_read = tm_resampler_calculate_input_samples(&source->resampler, samples_read);
            samples_read = source->buffer->funcs->request_samples(source, &srcleft, &srcright, samples_read);
            if (samples_read == 0) {
                // source is no longer playing
                source->flags &= ~TM_SOURCEFLAG_PLAYING;
                return;
            }

            samples_written = tm_resampler_calculate_output_samples(&source->resampler, samples_read);
            tm_resample_mono(&source->resampler, srcleft, samples_read, tm.scratch, samples_written);

            if (srcleft == srcright) {
                srcleft = tm.scratch;
                srcright = tm.scratch;
            } else {
                // resmample the second streo channel
                tm_resample_mono(&source->resampler, srcright, samples_read, tm.scratch + samples_written, samples_written);

                srcleft = tm.scratch;
                srcright = tm.scratch + samples_written;
            }
        } else {
            samples_read = source->buffer->funcs->request_samples(source, &srcleft, &srcright, samples_read);
            if (samples_read == 0) {
                // source is no longer playing
                source->flags &= ~TM_SOURCEFLAG_PLAYING;
                return;
            }
            samples_written = samples_read;
        }

        // render the source to the output mix
        for (int ii = 0; ii < samples_written; ++ii)
            *left++ += gain[0] * srcleft[ii];
        for (int ii = 0; ii < samples_written; ++ii)
            *right++ += gain[1] * srcright[ii];

        remaining -= samples_written;
    }
}

static void render_effects(float* buffer) {
    float compressor_factor = tm.compressor_factor;

    // get maximum absolute power level from the rendered buffer, and adjust the compressor factor
    float max_power = 0;
    for (int ii = 0; ii < N_SAMPLES; ++ii) {
        const float power = fabsf(buffer[ii]);
        if (power > max_power)
            max_power = power;
    }

    float target_compressor_factor = 1.0f;
    if (max_power > tm.compressor_thresholds[1])
        target_compressor_factor = tm.compressor_multipliers[1];
    else if (max_power > tm.compressor_thresholds[0])
        target_compressor_factor = tm.compressor_multipliers[0];

    float attack_release = 1.0f;
    if (target_compressor_factor < compressor_factor)
        attack_release = tm.compressor_attack_per1ksamples;
    else if (target_compressor_factor > compressor_factor)
        attack_release = tm.compressor_release_per1ksamples;

    // linearly interp compressor_factor toward the target compressor value
    const float interp = attack_release * N_SAMPLESF;
    compressor_factor = compressor_factor + interp*(target_compressor_factor - compressor_factor);
    compressor_factor = _tm_clamp(compressor_factor, tm.compressor_multipliers[1], 1.0f);

    // 2-pass compressor to limit dynamic range of audio clipping levels
    if (compressor_factor < 1.0f) {
        for (int cc = 0; cc < 2; ++cc) {
            float prev_sample = tm.compressor_last_samples[cc];

            float sample = 0;
            float* channel = buffer + cc*N_SAMPLES;
            for (int ii = 0; ii < N_SAMPLES; ++ii) {
                float sample = channel[ii];

                // uhhh... linear space? really??
                float diff = sample - prev_sample;
                sample = prev_sample + compressor_factor*diff;
                channel[ii] = sample;
            }

            tm.compressor_last_samples[cc] = sample;
        }
    }

    tm.compressor_factor = compressor_factor;
}

static void mix(float* buffer) {
    int nplaying = 0;
    int playing[N_SOURCES];
    float gain[N_SOURCES][2];

    // build active sounds
    for (int ii = 0; ii < N_SOURCES; ++ii) {
        const source_t* source = &tm.sources[ii];
        if (source->flags & TM_SOURCEFLAG_PLAYING) {
            playing[nplaying] = ii;
            ++nplaying;
        }
    }

    // Update source gains
    for (int ii = 0; ii < nplaying; ++ii) {
        const source_t* source = &tm.sources[playing[ii]];

        const float gain_base = tm.gain_master * tm.gain_base[source->gain_base_index] * source->gain_base;
        gain[ii][0] = gain_base;
        gain[ii][1] = gain_base;

        if (source->flags & TM_SOURCEFLAG_POSITIONAL) {
            const float dist = _tm_dist(tm.position, source->position);
            if (dist > 1.0e-8f) {
                // attenuation factor due to distance
                const float gain_distance = 1.0f - (dist - source->distance_min) / source->distance_difference;

                // attenuation factor due to panning (position audio)
                const float gain_panning = (source->position[0] - tm.position[0]) / dist;

                gain[ii][0] *= gain_distance * (1.0f + gain_panning * -SPEAKER_DIST);
                gain[ii][1] *= gain_distance * (1.0f + gain_panning * +SPEAKER_DIST);
            }
        }

        // clamp gains
        gain[ii][0] = _tm_clamp(gain[ii][0], 0.0f, 1.0f);
        gain[ii][1] = _tm_clamp(gain[ii][1], 0.0f, 1.0f);
    }

    memset(buffer, 0, sizeof(float)*2*N_SAMPLES);

    // render playing sources
    for (int ii = 0; ii < nplaying; ++ii) {
        source_t* source = &tm.sources[playing[ii]];
        render(source, buffer, gain[ii]);
    }

    // allow application to apply a premixed track (such as music)
    if (tm.callbacks.pre_effects)
        (tm.callbacks.pre_effects)(tm.callbacks.udata, buffer, N_SAMPLES, tm.gain_callback);

    // render effects
    render_effects(buffer);

    // perform source-level post processing
    for (int ii = 0; ii < nplaying; ++ii) {
        source_t* source = &tm.sources[playing[ii]];
        ++source->frame_age;

        // handle fadeout->stop
        if (source->flags & TM_SOURCEFLAG_FADEOUT) {
            source->gain_base -= source->fadeout_per_sample * N_SAMPLES;
            if (source->gain_base <= 0.0f) {
                source->flags = 0;
            }
        }
    }

    // cleanup dead sources
    for (int ii = 0; ii < nplaying; ++ii) {
        source_t* source = &tm.sources[playing[ii]];
        if (0 == (source->flags & TM_SOURCEFLAG_PLAYING)) {
            kill_source(source);
        }
    }
}

void tm_getsamples(float* samples, int nsamples) {
    mt_mutex_lock(&tm.lock);

    // was data leftover after the previous call to getsamples? Copy that out here
    while (nsamples && tm.samples_remaining) {
        const int samples_to_mix = _tm_min(nsamples, tm.samples_remaining);
        const int offset = N_SAMPLES - tm.samples_remaining;

        // clip and interleave
        for (int cc = 0; cc < 2; ++cc)
            for (int ii = 0; ii < samples_to_mix; ++ii)
                samples[cc + 2*ii] = _tm_clamp(tm.buffer[cc*N_SAMPLES + offset + ii], -1.0f, 1.0f);

        tm.samples_remaining -= samples_to_mix;
        samples += (2*samples_to_mix);
        nsamples -= samples_to_mix;
        mt_mutex_unlock(&tm.lock);
    }

    // Copy out samples
    while (nsamples) {
        const int samples_to_mix = _tm_min(nsamples, N_SAMPLES);
        mix(tm.buffer);
        tm.samples_remaining = N_SAMPLES;

        // clip and interleave
        for (int cc = 0; cc < 2; ++cc)
            for (int ii = 0; ii < samples_to_mix; ++ii)
                samples[cc + 2*ii] = _tm_clamp(tm.buffer[cc*N_SAMPLES + ii], -1.0f, 1.0f);


        tm.samples_remaining -= samples_to_mix;
        samples += (2*samples_to_mix);
        nsamples -= samples_to_mix;
    }

    mt_mutex_unlock(&tm.lock);
}

void tm_set_mastergain(float gain) {
    mt_mutex_lock(&tm.lock);
    tm.gain_master = gain;
    mt_mutex_unlock(&tm.lock);
}

static source_t* add(const tm_buffer* handle, int gain_index, float gain, float pitch) {
    source_t* source = find_source();
    if (!source)
        return 0;

    source->buffer = (const buffer_t*)handle;
    source->gain_base = gain;
    source->gain_base_index = (uint8_t)gain_index;
    source->frame_age = 0;

    const float diff = pitch - 1.0f;
    if (diff*diff < 1.0e-8f) {
        source->flags &= ~TM_SOURCEFLAG_FREQUENCY;
    } else {
        source->flags |= TM_SOURCEFLAG_FREQUENCY;
        tm_resampler_init_rate(&source->resampler, pitch);
    }

    source->buffer->funcs->start_source(source);

    _tm_addref((buffer_t*)handle);
    return source;
}

static void play(source_t* source) {
    source->flags |= TM_SOURCEFLAG_PLAYING;
}

typedef struct {
    buffer_t buffer;
    int32_t nsamples;
    uint8_t nchannels;
    // float smaples[nsamples*nschannels];
} static_sample_buffer;

static void static_sample_buffer_on_destroy(buffer_t* buffer) {
}

static void static_sample_buffer_start_source(source_t* source) {
    source->instance_data.static_source.sample_pos = 0;
}

static void static_sample_buffer_end_source(source_t*source) {
}

static int static_sample_buffer_request_samples(source_t* source, const float** left, const float** right, int nsamples) {
    const static_sample_buffer* buffer = (const static_sample_buffer*)source->buffer;

    int sample_pos = source->instance_data.static_source.sample_pos;

    // handle looping sources
    if (sample_pos == buffer->nsamples) {
        if (source->flags & TM_SOURCEFLAG_LOOPING) {
            sample_pos = 0;
        } else {
            return 0;
        }
    }

    nsamples = _tm_min(buffer->nsamples - sample_pos, nsamples);
    const float* srcleft = (float*)(buffer + 1) + sample_pos;

    *left = srcleft;
    if (buffer->nchannels == 1)
        *right = srcleft;
    else {
        *right = srcleft + buffer->nsamples;
    }

    source->instance_data.static_source.sample_pos = sample_pos + nsamples;
    return nsamples;
}

static int static_sample_buffer_get_buffer_size(const buffer_t* buffer) {
    const static_sample_buffer* sbuffer = (const static_sample_buffer*)buffer;
    return sizeof(static_sample_buffer) + sizeof(float)*sbuffer->nchannels*sbuffer->nsamples;
}

static buffer_functions_t static_sample_functions = {
    static_sample_buffer_on_destroy,
    static_sample_buffer_start_source,
    static_sample_buffer_end_source,
    static_sample_buffer_request_samples,
    static_sample_buffer_get_buffer_size,
};

void tm_create_buffer_interleaved_s16le(int channels, const int16_t* pcm_data, int pcm_data_size, const tm_buffer** handle) {
    const int nsamples = pcm_data_size/sizeof(uint16_t)/channels;

    static_sample_buffer* buffer = (static_sample_buffer*)tm.callbacks.allocate(tm.callbacks.udata, sizeof(buffer_t) + nsamples*channels*sizeof(float));
    buffer->buffer.funcs = &static_sample_functions;
    buffer->buffer.refcnt = 1;
    buffer->nchannels = (uint8_t)channels;
    buffer->nsamples = nsamples;

    // copy samples
    const int16_t* source = (const int16_t*)pcm_data;
    float* dest = (float*)(buffer + 1);
    for (int cc = 0; cc < channels; ++cc) {
        for (int ii = 0; ii < nsamples; ++ii)
            *dest++ = (float)source[channels*ii + cc] / (float)0x8000;
    }

    *handle = (tm_buffer*)buffer;
}

void tm_create_buffer_interleaved_float(int channels, const float* pcm_data, int pcm_data_size, const tm_buffer** handle) {
    const int nsamples = pcm_data_size/sizeof(float)/channels;

    static_sample_buffer* buffer = (static_sample_buffer*)tm.callbacks.allocate(tm.callbacks.udata, sizeof(buffer_t) + nsamples*channels*sizeof(float));
    buffer->buffer.funcs = &static_sample_functions;
    buffer->buffer.refcnt = 1;
    buffer->nchannels = (uint8_t)channels;
    buffer->nsamples = nsamples;

    // copy samples
    const float* source = (const float*)pcm_data;
    float* dest = (float*)(buffer + 1);
    for (int cc = 0; cc < channels; ++cc) {
        for (int ii = 0; ii < nsamples; ++ii)
            *dest++ = source[channels*ii + cc];//(int16_t)_tm_clamp((int32_t)(source[channels*ii + cc] * (float)0x8000), (int16_t)0x8000, 0x7fff);
    }

    *handle = (tm_buffer*)buffer;
}

typedef struct {
    buffer_t buffer;
    void* opaque;
    void (*closed)(void*);
    int ndata;
    // uint8_t vorbis_data[ndata]
} vorbis_stream_buffer;

static void vorbis_stream_on_destroy(buffer_t* buffer) {
    vorbis_stream_buffer* vbuffer = (vorbis_stream_buffer*)buffer;
    if (vbuffer->closed)
        vbuffer->closed(vbuffer->opaque);
}

static void vorbis_stream_start_source(source_t* source) {
    const vorbis_stream_buffer* vbuffer = (const vorbis_stream_buffer*)source->buffer;
    vorbis_stream_data_t* vsd = &source->instance_data.vorbis_stream;

    // open the vorbis stream
    unsigned char* data = (unsigned char*)(vbuffer + 1);
    vsd->v = stb_vorbis_open_memory(data, vbuffer->ndata, NULL, NULL);
    vsd->noutputs = 0;
}

static void vorbis_stream_end_source(source_t* source) {
    stb_vorbis* v = source->instance_data.vorbis_stream.v;
    if (v)
        stb_vorbis_close(v);

    source->instance_data.vorbis_stream.v = 0;
}

static int vorbis_stream_request_samples(source_t* source, const float** left, const float** right, int nsamples) {
    vorbis_stream_data_t* vsd = &source->instance_data.vorbis_stream;

    // no steam?
    if (!vsd->v)
        return 0;

    if (vsd->noutputs == 0) {
        int channels;
        float** outputs;
        vsd->noutputs = stb_vorbis_get_frame_float(vsd->v, &channels, &outputs);

        // if we're looping and have reached the end, seek to the start and try again
        if (vsd->noutputs == 0 && source->flags & TM_SOURCEFLAG_LOOPING) {
            stb_vorbis_seek_start(vsd->v);
            vsd->noutputs = stb_vorbis_get_frame_float(vsd->v, &channels, &outputs);
        }

        // handle mono streams
        if (vsd->noutputs) {
            vsd->outputs[0] = outputs[0];
            vsd->outputs[1] = (channels == 1) ? outputs[0] : outputs[1];
        }
    }

    nsamples = _tm_min(nsamples, vsd->noutputs);

    *left = vsd->outputs[0];
    *right = vsd->outputs[1];

    vsd->noutputs -= nsamples;
    vsd->outputs[0] += nsamples;
    vsd->outputs[1] += nsamples;
    return nsamples;
}

static int vorbis_stream_get_buffer_size(const buffer_t* buffer) {
    const vorbis_stream_buffer* vbuffer = (const vorbis_stream_buffer*)buffer;
    return sizeof(vorbis_stream_buffer) + vbuffer->ndata;
}

static buffer_functions_t vorbis_stream_buffer_funcs = {
    vorbis_stream_on_destroy,
    vorbis_stream_start_source,
    vorbis_stream_end_source,
    vorbis_stream_request_samples,
    vorbis_stream_get_buffer_size,
};

void tm_create_buffer_vorbis_stream(const void* data, int ndata, void* opaque, void (*closed)(void*), const tm_buffer** handle) {
    mt_mutex_lock(&tm.lock);
    vorbis_stream_buffer* buffer = (vorbis_stream_buffer*)tm.callbacks.allocate(tm.callbacks.udata, sizeof(vorbis_stream_buffer) + ndata);
    buffer->buffer.funcs = &vorbis_stream_buffer_funcs;
    buffer->buffer.refcnt = 1;
    buffer->opaque = opaque;
    buffer->closed = closed;
    buffer->ndata = ndata;

    // copy vorbis data
    memcpy(buffer + 1, data, ndata);
    *handle = (tm_buffer*)buffer;
    mt_mutex_unlock(&tm.lock);
}

typedef struct {
    buffer_t buffer;
    void* opaque;
    tm_buffer_callbacks callbacks;
} custom_stream_buffer;

static void custom_stream_on_destroy(buffer_t* buffer) {
    custom_stream_buffer* cbuffer = (custom_stream_buffer*)buffer;
    cbuffer->callbacks.on_destroy(cbuffer->opaque);
}

static void custom_stream_start_source(source_t* source) {
    custom_stream_buffer* cbuffer = (custom_stream_buffer*)source->buffer;
    source->instance_data.custom_stream.opaque = cbuffer->callbacks.start_source(cbuffer->opaque);
}

static void custom_stream_end_source(source_t* source) {
    custom_stream_buffer* cbuffer = (custom_stream_buffer*)source->buffer;
    cbuffer->callbacks.end_source(cbuffer->opaque, source->instance_data.custom_stream.opaque);
}

static int custom_stream_request_samples(source_t* source, const float** left, const float** right, int nsamples) {
    custom_stream_buffer* cbuffer = (custom_stream_buffer*)source->buffer;
    return cbuffer->callbacks.request_samples(cbuffer->opaque, source->instance_data.custom_stream.opaque, left, right, nsamples);
}

static int custom_stream_get_buffer_size(const buffer_t* buffer) {
    return sizeof(custom_stream_buffer);
}

static buffer_functions_t custom_stream_buffer_funcs = {
    custom_stream_on_destroy,
    custom_stream_start_source,
    custom_stream_end_source,
    custom_stream_request_samples,
    custom_stream_get_buffer_size,
};

void tm_create_buffer_custom_stream(void* opaque, tm_buffer_callbacks callbacks, const tm_buffer** handle) {
    mt_mutex_lock(&tm.lock);

    custom_stream_buffer* buffer = (custom_stream_buffer*)tm.callbacks.allocate(tm.callbacks.udata, sizeof(custom_stream_buffer));
    buffer->buffer.funcs = &custom_stream_buffer_funcs;
    buffer->buffer.refcnt = 1;
    buffer->opaque = opaque;
    buffer->callbacks = callbacks;

    *handle = (tm_buffer*)buffer;
    mt_mutex_unlock(&tm.lock);
}

int tm_get_buffer_size(const tm_buffer* handle) {
    mt_mutex_lock(&tm.lock);
    const buffer_t* buffer = (const buffer_t*)handle;
    mt_mutex_unlock(&tm.lock);
    return buffer->funcs->get_buffer_size(buffer);
}

void tm_release_buffer(const tm_buffer* handle) {
    mt_mutex_lock(&tm.lock);
    _tm_decref((buffer_t*)handle);
    mt_mutex_unlock(&tm.lock);
}

bool tm_add(const tm_buffer* handle, int gain_index, float gain, float pitch, tm_channel* channel) {
    mt_mutex_lock(&tm.lock);
    source_t* source = add(handle, gain_index, gain, pitch);
    if (source) {
        play(source);
        channel->index = (int)(source - tm.sources) + 1;
        mt_mutex_unlock(&tm.lock);
        return true;
    }
    mt_mutex_unlock(&tm.lock);
    return false;
}

bool tm_add_spatial(const tm_buffer* handle, int gain_index, float gain, float pitch, const float* position, float distance_min, float distance_max, tm_channel* channel) {
    mt_mutex_lock(&tm.lock);
    source_t *source = add(handle, gain_index, gain, pitch);
    if (source) {
        _tm_vcopy(source->position, position);
        source->flags |= TM_SOURCEFLAG_POSITIONAL;
        source->distance_min = distance_min;
        source->distance_difference = (distance_max - distance_min);
        play(source);
        channel->index = (int)(source - tm.sources) + 1;

        mt_mutex_unlock(&tm.lock);
        return true;
    }

    mt_mutex_unlock(&tm.lock);
    return false;
}


static source_t* _add_loop(const tm_buffer* handle, int gain_index, float gain, float pitch, tm_channel* channel) {
    source_t* source = add(handle, gain_index, gain, pitch);
    if (!source) {
        channel->index = 0;
        return 0;
    }

    source->flags |= TM_SOURCEFLAG_LOOPING;
    channel->index = (int)(source - tm.sources) + 1;
    return source;
}

bool tm_add_loop(const tm_buffer* handle, int gain_index, float gain, float pitch, tm_channel* channel) {
    mt_mutex_lock(&tm.lock);
    source_t* source = _add_loop(handle, gain_index, gain, pitch, channel);
    if (source) {
        play(source);

        mt_mutex_unlock(&tm.lock);
        return true;
    }

    mt_mutex_unlock(&tm.lock);
    return false;
}

bool tm_add_spatial_loop(const tm_buffer* handle, int gain_index, float gain, float pitch, const float* position, float distance_min, float distance_max, tm_channel* channel) {
    mt_mutex_lock(&tm.lock);
    source_t* source = _add_loop(handle, gain_index, gain, pitch, channel);
    if (source) {
        _tm_vcopy(source->position, position);
        source->flags |= TM_SOURCEFLAG_POSITIONAL;
        source->distance_min = distance_min;
        source->distance_difference = (distance_max - distance_min);

        play(source);

        mt_mutex_unlock(&tm.lock);
        return true;
    }

    mt_mutex_unlock(&tm.lock);
    return false;
}

void tm_channel_set_opaque(tm_channel channel, void* opaque) {
    mt_mutex_lock(&tm.lock);
    source_t* source = &tm.sources[channel.index - 1];
    source->opaque = opaque;
    mt_mutex_unlock(&tm.lock);
}

void tm_channel_stop(tm_channel channel) {
    mt_mutex_lock(&tm.lock);
    source_t* source = &tm.sources[channel.index - 1];
    kill_source(source);
    mt_mutex_unlock(&tm.lock);
}

void tm_channel_set_position(tm_channel channel, const float* position) {
    mt_mutex_lock(&tm.lock);
    source_t* source = &tm.sources[channel.index - 1];
    _tm_vcopy(source->position, position);
    mt_mutex_unlock(&tm.lock);
}

void tm_channel_fadeout(tm_channel channel, float seconds) {
    mt_mutex_lock(&tm.lock);
    source_t* source = &tm.sources[channel.index - 1];
    source->fadeout_per_sample = 1.0f / (seconds * tm.sample_rate);
    source->flags |= TM_SOURCEFLAG_FADEOUT;
}

void tm_channel_set_gain(tm_channel channel, float gain) {
    mt_mutex_lock(&tm.lock);
    source_t* source = &tm.sources[channel.index - 1];
    source->gain_base = gain;
    source->flags &= ~TM_SOURCEFLAG_FADEOUT;
    mt_mutex_unlock(&tm.lock);
}

float tm_channel_get_gain(tm_channel channel) {
    mt_mutex_lock(&tm.lock);
    source_t* source = &tm.sources[channel.index - 1];
    mt_mutex_unlock(&tm.lock);
    return source->gain_base;
}

void tm_channel_set_frequency(tm_channel channel, float frequency) {
    mt_mutex_lock(&tm.lock);
    source_t* source = &tm.sources[channel.index - 1];

    // clear frequency shift if ~0.0f
    const float diff = frequency - 1.0f;
    if (diff*diff < 1.0e-8f) {
        source->flags &= ~TM_SOURCEFLAG_FREQUENCY;
    } else {
        source->flags |= TM_SOURCEFLAG_FREQUENCY;
        source->resampler.ideal_rate = frequency;
    }
    mt_mutex_unlock(&tm.lock);
}

static void* _tm_default_allocate(void* opaque, int bytes) {
    return malloc(bytes);
}

static void _tm_default_free(void* opaque, void* ptr) {
    free(ptr);
}

void tm_init(tm_callbacks callbacks, int sample_rate) {
    // setup default callbacks where needed
    if (!callbacks.allocate) {
        callbacks.allocate = _tm_default_allocate;
    }
    if (!callbacks.free) {
        callbacks.free = _tm_default_free;
    }

    mt_mutex_init(&tm.lock);

    tm.gain_master = 1.0f;
    for (int ii = 0; ii < N_GAINTYPES; ++ii)
        tm.gain_base[ii] = 1.0f;

    tm.sample_rate = sample_rate;
    tm.callbacks = callbacks;
    tm.samples_remaining = 0;

    const float default_thresholds[2] = {1.0f, 1.0f};
    const float default_multipliers[2] = {1.0f, 1.0f};
    const float default_attack = 0.0f;
    const float default_release = 0.0f;
    tm_effects_compressor(default_thresholds, default_multipliers, default_attack, default_release);
    tm.compressor_factor = 1.0f;
    tm.compressor_last_samples[0] = tm.compressor_last_samples[1] = 0;
}

void tm_shutdown() {
    mt_mutex_destroy(&tm.lock);
}

void tm_update_listener(const float* position) {
    mt_mutex_lock(&tm.lock);
    _tm_vcopy(tm.position, position);
    mt_mutex_unlock(&tm.lock);
}

void tm_set_base_gain(int index, float gain) {
    mt_mutex_lock(&tm.lock);
    tm.gain_base[index] = gain;
    mt_mutex_unlock(&tm.lock);
}

void tm_set_callback_gain(float gain) {
    mt_mutex_lock(&tm.lock);
    tm.gain_callback = gain;
    mt_mutex_unlock(&tm.lock);
}

void tm_effects_compressor(const float thresholds[2], const float multipliers[2], float attack_seconds, float release_seconds) {
    mt_mutex_lock(&tm.lock);
    tm.compressor_thresholds[0] = _tm_clamp(thresholds[0], 0.0f, 1.0f);
    tm.compressor_thresholds[1] = _tm_clamp(thresholds[1], 0.0f, 1.0f);
    tm.compressor_multipliers[0] = _tm_clamp(multipliers[0], 0.0f, 1.0f);
    tm.compressor_multipliers[1] = _tm_clamp(multipliers[1], 0.0f, 1.0f);

    float attackSampleRate = (attack_seconds * (float)tm.sample_rate);
    tm.compressor_attack_per1ksamples = (attackSampleRate > 0.0f) ? (1.0f / attackSampleRate) : 1.0f;

    float releaseSampleRate = (release_seconds * (float)tm.sample_rate);
    tm.compressor_release_per1ksamples = (releaseSampleRate > 0.0f) ? (1.0f / releaseSampleRate) : 1.0f;
    mt_mutex_unlock(&tm.lock);
}

void tm_stop_all_sources() {
    mt_mutex_lock(&tm.lock);
    for (int ii = 0; ii < N_SOURCES; ++ii) {
        source_t* source = &tm.sources[ii];
        if (source->buffer) {
            kill_source(source);
        }
    }
    mt_mutex_unlock(&tm.lock);
}

void tm_resampler_init(tm_resampler* resampler, int input_sample_rate, int output_sample_rate) {
    const float ideal_rate = (float)input_sample_rate / (float)output_sample_rate;
    tm_resampler_init_rate(resampler, ideal_rate);
}

void tm_resampler_init_rate(tm_resampler* resampler, float ideal_rate) {
    resampler->ideal_rate = ideal_rate;
    resampler->prev_samples[0] = 0.0f;
    resampler->prev_samples[1] = 0.0f;
}

int tm_resampler_calculate_input_samples(const tm_resampler* resampler, int output_samples) {
    const float input_samples = ceilf(resampler->ideal_rate * (float)output_samples);
    return (int)input_samples;
}

int tm_resampler_calculate_output_samples(const tm_resampler* resampler, int input_samples) {
    const float output_samples = ceilf((float)input_samples / resampler->ideal_rate);
    return (int)output_samples;
}

void tm_resample_stereo(tm_resampler* resampler, const float* input, int num_input_samples, float* output, int num_output_samples) {
    float* output_end = output + (2 * num_output_samples) - 2;

    float pos = resampler->ideal_rate;
    while (pos < 1.0f) {
        output[0] = resampler->prev_samples[0] + pos * (input[0] - resampler->prev_samples[0]);
        output[1] = resampler->prev_samples[1] + pos * (input[1] - resampler->prev_samples[1]);

        output += 2;
        pos += resampler->ideal_rate;
    }

    while (output < output_end) {
        const float pos_floor = floorf(pos);
        const int index = 2 * (int)pos_floor;
        output[0] = input[index - 2] + (input[index + 0] - input[index - 2]) * (pos - pos_floor);
        output[1] = input[index - 1] + (input[index + 1] - input[index - 1]) * (pos - pos_floor);

        output += 2;
        pos += resampler->ideal_rate;
    }

    output[0] = input[2 * num_input_samples - 2];
    output[1] = input[2 * num_input_samples - 1];

    resampler->prev_samples[0] = output[0];
    resampler->prev_samples[1] = output[1];
}

void tm_resample_mono(tm_resampler* resampler, const float* input, int num_input_samples, float* output, int num_output_samples) {
    float* output_end = output + num_output_samples - 1;

    float pos = resampler->ideal_rate;
    while (pos < 1.0f) {
        output[0] = resampler->prev_samples[0] + pos * (input[0] - resampler->prev_samples[0]);

        ++output;
        pos += resampler->ideal_rate;
    }

    while (output < output_end) {
        const float pos_floor = floorf(pos);
        const int index = (int)pos_floor;
        output[0] = input[index - 1] + (input[index] - input[index - 1]) * (pos - pos_floor);

        ++output;
        pos += resampler->ideal_rate;
    }

    output[0] = input[num_input_samples - 1];

    resampler->prev_samples[0] = output[0];
}

void* tm_vorbis_malloc(size_t sz) {
    return tm.callbacks.allocate(tm.callbacks.udata, (int)sz);
}

void tm_vorbis_free(void* ptr) {
    tm.callbacks.free(tm.callbacks.udata, ptr);
}

void* tm_vorbis_temp_malloc(size_t sz) {
    return tm_vorbis_malloc((int)sz);
}

void tm_vorbis_temp_free(void* ptr) {
    tm_vorbis_free(ptr);
}

void tm_lowpass_filter_init(tm_lowpass_filter* filter, float cutoff_frequency, float sample_rate) {
    filter->cutoff_frequency = cutoff_frequency;
    filter->sample_rate = sample_rate;

    memset(filter->channel_history, 0, sizeof(filter->channel_history));
}

void tm_lowpass_filter_apply(tm_lowpass_filter* filter, float* output, float* input, int num_samples, int num_channels) {

    float yk[2] = {
          filter->channel_history[0]
        , filter->channel_history[1]
    };

    const float alpha = filter->cutoff_frequency / filter->sample_rate;

    for (int ii = 0; ii != num_samples; ++ii) {
        for (int channel = 0; channel != num_channels; ++channel) {
            yk[channel] += alpha * (input[ii*num_channels + channel] - yk[channel]);
            output[ii*num_channels + channel] = yk[channel];
        }
    }

    filter->channel_history[0] = yk[0];
    filter->channel_history[1] = yk[1];
}

#endif //TMIXER_IMPL
