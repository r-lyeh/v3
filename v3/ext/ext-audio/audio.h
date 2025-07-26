// audio framework
// - rlyeh, public domain

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#define DR_FLAC_IMPLEMENTATION
#include "dr_flac.h"

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

#include "jo_mp1.h"

#define error stb_vorbis_error
#define get_bits stb_vorbis_get_bits
#include "stb_vorbis.h"
#undef  get_bits
#undef  error
#undef  C
#undef  L
#undef  R

#define channel jar_channel
#define JAR_MOD_IMPLEMENTATION              // jar_mod
#include "jar_mod.h"
#undef DEBUG
#undef channel
#define JAR_XM_IMPLEMENTATION               // jar_xm
#include "jar_xm.h"

#define STS_MIXER_IMPLEMENTATION            // sts_mixer
#include "sts_mixer.h"

#define SOKOL_AUDIO_IMPL                    // sokol_audio
#include "sokol_audio.h"

// -----------------------------------------------------------------------------
// encapsulate drflac and some buffer with the sts_mixer_stream_t
enum { UNK, WAV, MOD, XM, FLAC, OGG, MP1, MP3 };
typedef struct {
    int type;
    union {
        drwav wav;
        jar_mod_context_t mod;
        jar_xm_context_t *xm;
        stb_vorbis *ogg;
        void *opaque;
        drflac*         flac;               // FLAC decoder state
        drmp3           mp3_;
    };
    sts_mixer_stream_t  stream;             // mixer stream
    union {
    int32_t             data[4096*2];       // static sample buffer
    float               dataf[4096*2];
    };
} mystream_t;

static void stereo_float_to_mono( int channels, float *buffer, int samples ) {
    if( channels > 1 ) {
        float mix;
        float *output = buffer;
        while( samples-- > 0 ) {
            float mix = 0;
            for( int i = 0; i < channels; ++i ) mix += *buffer++;
            *output++ = (float)(mix / channels);
        }
    }
}
static void stereo_s16_to_mono( int channels, short *buffer, int samples ) {
    if( channels > 1 ) {
        float mix;
        short *output = buffer;
        while( samples-- > 0 ) {
            float mix = 0;
            for( int i = 0; i < channels; ++i ) mix += *buffer++;
            *output++ = (short)(mix / channels);
        }
    }
}

// the callback to refill the (stereo) stream data
static void refill_stream(sts_mixer_sample_t* sample, void* userdata) {
    mystream_t* stream = (mystream_t*)userdata;
    switch( stream->type ) {
        default:
        break; case FLAC: {
            int sl = sample->length / 2; /*sample->channels*/;
            if (drflac_read_pcm_frames_f32(stream->flac, sl, stream->dataf) < sl) {
                drflac_seek_to_pcm_frame(stream->flac, 0);
            }
        }
        break; case WAV: {
            if (drwav_read_pcm_frames_s16(&stream->wav, sample->length, (short*)stream->data) < sample->length) {
                drwav_seek_to_pcm_frame(&stream->wav, 0);
            }
        }
        break; case MP3: {
            int sl = sample->length / 2; /*sample->channels*/;
            if (drmp3_read_pcm_frames_f32(&stream->mp3_, sl, stream->dataf) < sl) {
                drmp3_seek_to_pcm_frame(&stream->mp3_, 0);
            }
        }
        break; case OGG: {
            stb_vorbis *ogg = (stb_vorbis*)stream->ogg;
            if( stb_vorbis_get_samples_short_interleaved(ogg, 2, (short*)stream->data, sample->length) == 0 )  {
                stb_vorbis_seek(stream->ogg, 0);
            }
        }
        break; case MOD: {
            jar_mod_context_t *mod = (jar_mod_context_t*)&stream->mod;
            jar_mod_fillbuffer(mod, (short*)stream->data, sample->length / 2, 0);
        }
        break; case XM: {
            jar_xm_context_t *xm = (jar_xm_context_t*)stream->xm;
            jar_xm_generate_samples_16bit(xm, (short*)stream->data, sample->length / 2 );
        }
    }
}

// load a (stereo) stream
static bool load_stream(mystream_t* stream, const char *filename) {
    filename = path_find(filename);

    int error;
    int HZ = 44100;
    stream->type = UNK;
    if( stream->type == UNK && (stream->ogg = stb_vorbis_open_filename(filename, &error, NULL)) ) {
        stb_vorbis_info info = stb_vorbis_get_info(stream->ogg);
        if( info.channels != 2 ) { lputs("\6cannot stream ogg file. stereo required."); goto end; }
        stream->type = OGG;
        stream->stream.sample.frequency = info.sample_rate;
        stream->stream.sample.audio_format = STS_MIXER_SAMPLE_FORMAT_16;
    }
    if( stream->type == UNK && (drwav_init_file(&stream->wav, filename, NULL))) {
        if( stream->wav.channels != 2 ) { lputs("\6cannot stream wav file. stereo required."); goto end; }
        stream->type = WAV;
        stream->stream.sample.frequency = stream->wav.sampleRate;
        stream->stream.sample.audio_format = STS_MIXER_SAMPLE_FORMAT_16;
    }
    if( stream->type == UNK && (stream->flac = drflac_open_file(filename, NULL)) ) {
        if( stream->flac->channels != 2 ) { lputs("\6cannot stream flac file. stereo required."); goto end; }
        stream->type = FLAC;
        stream->stream.sample.frequency = stream->flac->sampleRate;
        stream->stream.sample.audio_format = STS_MIXER_SAMPLE_FORMAT_FLOAT;
    }
    if( stream->type == UNK && (jar_xm_create_context_from_file(&stream->xm, HZ, filename) == 0)) {
        stream->type = XM;
        stream->stream.sample.frequency = HZ;
        stream->stream.sample.audio_format = STS_MIXER_SAMPLE_FORMAT_16;
    }
    if( stream->type == UNK && ((jar_mod_init(&stream->mod), jar_mod_load_file(&stream->mod, filename)) != 0) ) {
        stream->type = MOD;
        jar_mod_setcfg(&stream->mod, HZ, 16/*bits*/, 1/*stereo*/, 1/*stereo_separation*/, 1/*filter*/);
        stream->stream.sample.frequency = HZ;
        stream->stream.sample.audio_format = STS_MIXER_SAMPLE_FORMAT_16;
    }
    drmp3_config mp3_cfg = { 2, HZ };
    if( stream->type == UNK && (drmp3_init_file(&stream->mp3_, filename, NULL/*&mp3_cfg*/) != 0) ) {
        stream->type = MP3;
        stream->stream.sample.frequency = stream->mp3_.sampleRate;
        stream->stream.sample.audio_format = STS_MIXER_SAMPLE_FORMAT_FLOAT;
    }

    if( stream->type == UNK ) {
        return false;
    }

    end:;
    stream->stream.userdata = stream;
    stream->stream.callback = refill_stream;
    stream->stream.sample.length = sizeof(stream->data) / sizeof(stream->data[0]);
    stream->stream.sample.data = stream->data;
    refill_stream(&stream->stream.sample, stream);

    return true;
}

// load a (mono) sample
static bool load_sample(sts_mixer_sample_t* sample, const char *filename) {
    filename = path_find(filename);

    int error;
    int channels = 0;
    if( !channels ) for( drwav w = {0}, *wav = &w; wav && drwav_init_file(wav, filename, NULL); wav = 0 ) {
        channels = wav->channels;
        sample->frequency = wav->sampleRate;
        sample->audio_format = STS_MIXER_SAMPLE_FORMAT_16;
        sample->length = wav->totalPCMFrameCount;
        sample->data = REALLOC(0, sample->length * sizeof(short) * channels);
        drwav_read_pcm_frames_s16(wav, sample->length, (short*)sample->data);
        drwav_uninit(wav);
    }
    if( !channels ) for( stb_vorbis *ogg = stb_vorbis_open_filename(filename, &error, NULL); ogg; ogg = 0 ) {
        stb_vorbis_info info = stb_vorbis_get_info(ogg);
        channels = info.channels;
        sample->frequency = info.sample_rate;
        sample->audio_format = STS_MIXER_SAMPLE_FORMAT_16;
        sample->length = (int)stb_vorbis_stream_length_in_samples(ogg);
        stb_vorbis_close(ogg);

        short *buffer;
        int sample_rate;
        stb_vorbis_decode_filename(filename, &channels, &sample_rate, (short **)&buffer);
        sample->data = buffer;
    }
    if( !channels ) for( drflac* flac = drflac_open_file(filename, NULL); flac; flac = 0 ) {
        channels = flac->channels;
        sample->frequency = flac->sampleRate;
        sample->audio_format = STS_MIXER_SAMPLE_FORMAT_16;
        sample->length = flac->totalPCMFrameCount;
        sample->data = REALLOC(0, sample->length * sizeof(short) * channels);
        drflac_read_pcm_frames_s16(flac, sample->length, (short*)sample->data);
        drflac_close(flac);
    }
    drmp3_config mp3_cfg = { 2, 44100 };
    drmp3_uint64 mp3_fc;
    if( !channels ) for( short *fbuf = 0; fbuf = drmp3_open_file_and_read_pcm_frames_s16(filename, &mp3_cfg, &mp3_fc, NULL); ) {
        channels = mp3_cfg.channels;
        sample->frequency = mp3_cfg.sampleRate;
        sample->audio_format = STS_MIXER_SAMPLE_FORMAT_16;
        sample->length = mp3_fc; //  / sizeof(float) / mp3_cfg.channels;
        sample->data = fbuf;
        break;
    }
    if( !channels ) {
        file_t f = file_read(filename);
        if( f.bin && f.len ) {
            short *output = 0;
            int outputSize, hz, mp1channels;
            bool ok = jo_read_mp1(f.bin, f.len, &output, &outputSize, &hz, &mp1channels);
            if( ok ) {
                channels = mp1channels;
                sample->frequency = hz;
                sample->audio_format = STS_MIXER_SAMPLE_FORMAT_16;
                sample->length = outputSize / sizeof(int16_t) / channels;
                sample->data = REALLOC(0, sample->length * sizeof(int16_t) * channels );
                memcpy( sample->data, output, outputSize );
            }
        }
    }

    if( !channels ) {
        return false;
    }

    if( channels > 1 ) {
        if( sample->audio_format == STS_MIXER_SAMPLE_FORMAT_FLOAT ) {
            stereo_float_to_mono( channels, sample->data, sample->length );
            sample->data = REALLOC( sample->data, sample->length * sizeof(float));
        }
        else
        if( sample->audio_format == STS_MIXER_SAMPLE_FORMAT_16 ) {
            stereo_s16_to_mono( channels, sample->data, sample->length );
            sample->data = REALLOC( sample->data, sample->length * sizeof(short));
        }
        else {
            lputs("error!");
        }
    }

    return true;
}

// -----------------------------------------------------------------------------

sts_mixer_t mixer;

// This is the function that's used for sending more data to the device for playback.
static void audio_callback(float* buffer, int num_frames, int num_channels, void* user_data) {
    int len = num_frames; // * num_channels;
    sts_mixer_mix_audio(&mixer, buffer, len);
}

void audio_drop(void) {
    ONCE saudio_shutdown();
}

int audio_init( int flags ) {
    ONCE {
    // init sts_mixer
    sts_mixer_init(&mixer, 44100, STS_MIXER_SAMPLE_FORMAT_FLOAT);

    saudio_desc desc = {0};
//  desc.sample_rate = AUDIO_FREQUENCY;
//  desc.buffer_frames =   (AUDIO_BUFFERLEN/8); // number of frames in the buffer (2048: ~46.4ms)
//  desc.packet_frames = (AUDIO_BUFFERLEN/128); // number of packets per thread transfer
//  desc.num_packets =   (AUDIO_BUFFERLEN/256); // number of packets in ring buffer
    desc.num_channels = 2;
    desc.stream_userdata_cb = audio_callback;
    saudio_setup(&desc);
    atexit(audio_drop);

    lputs(va("\4audio\7: %dHz rate, %d channels, %d frames", saudio_sample_rate(), saudio_channels(), saudio_buffer_frames()));

//  if( saudio_sample_rate() != AUDIO_FREQUENCY || saudio_channels() != AUDIO_CHANNELS )
//      alert(va("Cannot initialize audio [%dHz,%dch]", saudio_sample_rate(), saudio_channels())),
//      exit(-1);

    }

    return true;
}

typedef struct audio_handle {
    bool is_clip;
    bool is_stream;
    union {
    sts_mixer_sample_t clip;
    mystream_t         stream;
    };
} audio_handle;

void audio_destroy_(void *o) {
    audio_handle *a = (audio_handle *)o;
    if( a && a->is_stream ) {
        mystream_t *ss = &a->stream;
        if( ss->type == WAV ) drwav_uninit(&ss->wav);
        if( ss->type == MOD ) jar_mod_unload(&ss->mod);
        if( ss->type == XM && ss->xm ) jar_xm_free_context(ss->xm);
    }
}

audio_t audio_clip_( const char *pathfile ) {
    ONCE audio_init(0);
    audio_handle *a = make(sizeof(audio_handle), audio_destroy_);
    a->is_clip = load_sample( &a->clip, pathfile );
    return a;
}
audio_t audio_stream_( const char *pathfile ) {
    ONCE audio_init(0);
    audio_handle *a = make(sizeof(audio_handle), audio_destroy_);
    a->is_stream = load_stream( &a->stream, pathfile );
    return a;
}

int audio_play_( audio_t a, int flags ) {
    if( a->is_clip ) {
        float gain = 1.0f; // randf(); // [0..1]
        float pitch = 1.f; // (0..N]
        float pan =  0; // -1.0f + randf() * 2.0f; // [-1..+1]
        int voice = sts_mixer_play_sample(&mixer, &a->clip, gain, pitch, pan);
        if( voice == -1 ) return 0; // all voices busy
    }
    else if( a->is_stream ) {
        float gain = 0.5f;
        int voice = sts_mixer_play_stream(&mixer, &a->stream.stream, gain);
        if( voice == -1 ) return 0; // all voices busy
    }
    return 1;
}
