#define DR_WAV_IMPLEMENTATION
#include "3rd/dr_wav.h"

#define DR_MP3_IMPLEMENTATION
#include "3rd/dr_mp3.h"

#if 0
#define DR_FLAC_IMPLEMENTATION
#include "3rd/dr_flac.h"

#define JO_MP1_IMPLEMENTATION
#include "3rd/jo_mp1.h"

#define JO_WAV_IMPLEMENTATION
#include "3rd/jo_wav.h"

#define JAR_MOD_IMPLEMENTATION              // jar_mod
#include "3rd/jar_mod.h"

#define JAR_XM_IMPLEMENTATION               // jar_xm
#include "3rd/jar_xm.h"
#endif

//#define STB_VORBIS_HEADER_ONLY
#define error stbv_error
#include "3rd/stb_vorbis.h"
#undef  error
#undef  L
#undef  R
#undef  C

// by default, we keep memory requirements small:
// - we decode to 16-bits (we can upscale to float later).
// - we leave channels as-is (we can upscale to stereo later).
// - we leave frequency as-is (we can upscale to 44khz later).
audio_type audio_decode(file_t f) {
    audio_type t = {0};

    //if( !t.channels ) for( drwav w = {0}, *wav = &w; wav && drwav_init_file(wav, filename, NULL); wav = 0 ) {
    if( !t.channels ) for( drwav w = {0}, *wav = &w; wav && drwav_init_memory(wav, f.bin, f.len, NULL); wav = 0 ) {
        t.channels = wav->channels;
        t.frequency = wav->sampleRate;
        t.bits = 16;
        t.count = wav->totalPCMFrameCount;
        t.data = realloc(0, t.count * sizeof(short) * t.channels);
        drwav_read_pcm_frames_s16(wav, t.count, (short*)t.data);
        drwav_uninit(wav);
    }
    int error;
    //if( !t.channels ) for( stb_vorbis *ogg = stb_vorbis_open_filename(filename, &error, NULL); ogg; ogg = 0 ) {
    if( !t.channels ) for( stb_vorbis *ogg = stb_vorbis_open_memory((const unsigned char *)f.bin, f.len, &error, NULL); ogg; ogg = 0 ) {
        stb_vorbis_info info = stb_vorbis_get_info(ogg);
        t.channels = info.channels;
        t.frequency = info.sample_rate;
        t.bits = 16;
        t.count = (int)stb_vorbis_stream_length_in_samples(ogg);
        stb_vorbis_close(ogg);

        short *buffer;
        int sample_rate;
        //stb_vorbis_decode_filename(filename, &t.channels, &sample_rate, (short **)&buffer);
        stb_vorbis_decode_memory((const unsigned char *)f.bin, f.len, &t.channels, &sample_rate, (short **)&buffer);
        t.data = buffer;
    }
    drmp3_config mp3_cfg = { 2, 44100 };
    drmp3_uint64 mp3_fc;
    //if( !t.channels ) for( short *fbuf = drmp3_open_file_and_read_pcm_frames_s16(filename, &mp3_cfg, &mp3_fc, NULL); fbuf; fbuf = 0 ) {
    if( !t.channels ) for( short *fbuf = drmp3_open_memory_and_read_pcm_frames_s16(f.bin, f.len, &mp3_cfg, &mp3_fc, NULL); fbuf ; fbuf = 0 ) {
        t.channels = mp3_cfg.channels;
        t.frequency = mp3_cfg.sampleRate;
        t.bits = 16;
        t.count = mp3_fc; //  / sizeof(float) / mp3_cfg.channels;
        t.data = fbuf;
    }
#if 0
    if( !t.channels ) for( drflac* flac = drflac_open_file(filename, NULL); flac; flac = 0 ) {
        t.channels = flac->channels;
        t.frequency = flac->sampleRate;
        t.bits = 16;
        t.count = flac->totalPCMFrameCount;
        t.data = realloc(0, t.count * sizeof(short) * t.channels);
        drflac_read_pcm_frames_s16(flac, t.count, (short*)t.data);
        drflac_close(flac);
    }
    if( !t.channels ) {
        short *output = 0;
        int outputSize, hz, mp1channels;
        bool ok = jo_read_mp1(f.bin, f.len, &output, &outputSize, &hz, &mp1channels);
        if( ok ) {
            t.channels = mp1channels;
            t.frequency = hz;
            t.bits = 16;
            t.count = outputSize / sizeof(int16_t) / t.channels;
            t.data = output; // realloc(0, t.count * sizeof(int16_t) * t.channels );
            // memcpy( t.data, output, outputSize );
        }
    }
#endif

    return t;
}
