#ifndef AUDIO_H
#define AUDIO_H

typedef struct audio_handle* audio_t;

extern audio_t (*audio_clip)( const char *pathfile );
extern audio_t (*audio_stream)( const char *pathfile );
extern     int (*audio_play)( audio_t s, int flags );

#elif CODE
#pragma once
#include v3_audio_backend

audio_t (*audio_clip)( const char * ) = audio_clip_;
audio_t (*audio_stream)( const char * ) = audio_stream_;
    int (*audio_play)( audio_t s, int  ) = audio_play_;

#endif
