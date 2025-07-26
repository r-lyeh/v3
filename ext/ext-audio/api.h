#ifndef AUDIO_H
#define AUDIO_H

// @todo: move this over to scene
typedef struct node3 { vec3 position; } node3;

typedef struct audio_t {
    void *vt;
    const void *handle;
    const char *url;
    const void *bin; int len;
    ifndef(V3_RETAIL, char debug[64];)
} audio_t;

V3_API extern audio_t (*audio)(audio_t f);
V3_API extern void    (*audio_drop)(audio_t* f);

#define audio(...) audio(CAST(audio_t) { .vt = 0, ##__VA_ARGS__ })

typedef struct speaker_t {
    void *vt;
    audio_t source;
    bool loop;
    // Pointing `node` field to a float[3] will make the sound spatial.
    // In this case, you have to fill in `range_min` and `range_max` as well.
    float* node, node_; float range_min, range_max;
    // impl
    void *opaque;
} speaker_t;

V3_API extern speaker_t (*speaker)(speaker_t s);
V3_API extern void      (*speaker_play)(speaker_t *s);
V3_API extern void      (*speaker_stop)(speaker_t *s);
V3_API extern void      (*speaker_drop)(speaker_t *s);

#define speaker(...) speaker(CAST(speaker_t) { .vt = 0, ##__VA_ARGS__ })

// optional update for all audio dynamics. statics do not need further update

typedef struct listener_t {
    vec3 position, position_;
    float smoothing; // Around 2 is a good starting point.
} listener_t;

#define listener(...) ctor(listener_t, ##__VA_ARGS__)[0]

void audio_update(listener_t *l,int num_spatial_speakers, speaker_t *spatials, float dt);

V3_API extern int (*ui_audio)(void);
V3_API extern int (*ui_audio_t)(audio_t);

#elif CODE
#pragma once
#include v3_audio_backend

audio_t   (*audio)(audio_t) = audio_;
void      (*audio_drop)(audio_t*) = audio_drop_;

speaker_t (*speaker)(speaker_t) = speaker_;
void      (*speaker_play)(speaker_t*) = speaker_play_;
void      (*speaker_stop)(speaker_t*) = speaker_stop_;
void      (*speaker_drop)(speaker_t*) = speaker_drop_;

int       (*ui_audio)(void) = ui_audio_;
int       (*ui_audio_t)(audio_t) = ui_audio_t_;

#endif
