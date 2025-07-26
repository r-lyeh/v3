// audio framework. Based on code by @arnkoen (uLicensed)
// - rlyeh, public domain

#include "audio_convert.h"
#include "audio_decoder.h"

//#include "../ext-thread/3rd/minithread.h"

#define TMIXER_IMPL
#include "3rd/tmixer.h"

static
char audio_details[128];

int ui_audio_(void) { return 0; }
int ui_audio_t_(audio_t a) { return 0; }

audio_t audio_(audio_t f) {
    ONCE lputs(audio_details);

    if( !f.handle && f.url ) {
        ifndef(V3_RETAIL, strncpy(f.debug, f.url, COUNTOF(f.debug)));
        file_t x = asset(f.url);
        f.bin = x.bin; f.len = x.len;
    }
    if( !f.handle && f.bin && f.len > 4 ) {
        const tm_buffer *handle = NULL;
        #if 0
        if( !memcmp(f.bin, "OggS", 4)) {
            tm_create_buffer_vorbis_stream(f.bin, f.len, NULL, NULL, &handle);
        }
        #endif
        if( !handle ) {
            file_t x = {(void*)f.bin, f.len};
            audio_type t = audio_decode(x);

            if(t.frequency != saudio_sample_rate()) {
                audio_type out = t;
                out.frequency = saudio_sample_rate();
                bool ok = audio_convert(&t.data, t.count, t, out, NULL);
            }

            if( t.bits == 16 ) tm_create_buffer_interleaved_s16le(t.channels, t.data, t.count * sizeof(short) * t.channels, &handle); // t.bytes?
            if( t.bits ==-32 ) tm_create_buffer_interleaved_float(t.channels, t.data, t.count * sizeof(float) * t.channels, &handle); // t.bytes?
        }
        f.handle = handle;
    }
    return f;
}

void audio_drop_(audio_t *a) {
    tm_release_buffer((const tm_buffer *)a->handle);
}

// --------------------------------------------------------------------

void speaker_play_(speaker_t* s) {
    assert(s->source.handle);

    if(!s->opaque) {
        s->opaque = calloc(1, sizeof(tm_channel));
    }

    struct tm_channel *channel = s->opaque;

    int gain_index = 0;
    float gain = 1.0f;
    float pitch = 1.0f; //0.5f;

    // check if we need positional audio
    if( !s->node )
    (s->loop ? tm_add_loop : tm_add)(s->source.handle, gain_index, gain, pitch, channel);
    else
    (s->loop ? tm_add_spatial_loop : tm_add_spatial)(s->source.handle, gain_index, gain, pitch, (const float*)s->node, s->range_min, s->range_max, channel);
}

void speaker_stop_(speaker_t* s) {
    if(s->opaque) tm_channel_stop(*(tm_channel *)s->opaque);
}

speaker_t speaker_(speaker_t s) {
    return speaker_play_(&s), s;
}

static
void speaker_update(speaker_t* s) {
    if( s->node && s->opaque ) {
        tm_channel_set_position(*(tm_channel *)s->opaque, (const float *)s->node);
    }
}

void speaker_drop_(speaker_t *s) {
    speaker_stop_(s);
    if(s->opaque) free(s->opaque);
}

// --------------------------------------------------------------------

static
void listener_update(listener_t* l, float dt) {
    // @todo: infer velocity `sub3(pos_,pos)/dt` and direction `norm3(vel)` too
    const float smoothing = 1.0f - expf(-dt * l->smoothing);
    l->position = lerp3(l->position, l->position_, smoothing);
    tm_update_listener(&l->position.x);
    l->position_ = l->position;
}

void audio_update(listener_t *l,int num_spatial_speakers, speaker_t *spatials, float dt) {
    listener_update(l,dt);
    for( int i = 0; i < num_spatial_speakers; ++i) speaker_update(&spatials[i]);
}

// --------------------------------------------------------------------

static void audio_stream_cb(float* buffer, int num_frames, int num_channels, void* udata) {
    (void)num_channels; (void)udata;
    tm_getsamples(buffer, num_frames);
}

void audio_quit(void) {
    if( saudio_isvalid() ) {
        tm_shutdown();
        saudio_shutdown();
    }
}
void audio_init(void) {
    if(!saudio_isvalid()) {
        atexit(audio_quit);
        saudio_desc ad = { 0 };
        ad.num_channels = 2;
        ad.stream_userdata_cb = audio_stream_cb;
        saudio_setup(&ad);

        static tm_callbacks mixer_callbacks;
        tm_init(mixer_callbacks, saudio_sample_rate());

        snprintf(audio_details, sizeof(audio_details), "%saudio: %dKHz, %d channels, %d frames", saudio_isvalid() ? "" : "Cannot init ", saudio_sample_rate()/1000, saudio_channels(), saudio_buffer_frames());
    }
}

AUTORUN {
    audio_init();
}
