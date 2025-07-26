#ifndef APP_H
#define APP_H

// public api

typedef struct {
    const char *title, *extra;
    union { int w, width; };
    union { int h, height; };
    union { float s, scale; };
    union { float a, aspect; };
    bool visible, minimized, rotated;
    bool transparent, resizable;
    bool debug, silent; // gl: debug context and whether to report errors or not
    union { unsigned aa, msaa, samples; }; //gl
    // icon, icon_small
    // good ideas? fullscreen, depth:32/24, stencil:8, srgb
} app_create_flags, app_create_flags_t;

typedef struct {
    int hz;         // <0: adaptive, 0: max, 1: vsync .. NN: NN Hz
    float depth;    // <0: do not clear depth,   [0..1] clear depth
    int stencil;    // <0: do not clear stencil, [0..N] clear stencil
    unsigned color; // -1: do not clear color,   [XXXX] clear color
} app_swap_flags, app_swap_flags_t;

typedef struct {
    char *extra;
    union { int w, width; };
    union { int h, height; };
    union { float a, aspect; };
    void *handle;
    void *handle_os;
} app_state_t;

bool        app_create( app_create_flags_t opt );
bool        app_swap( app_swap_flags_t opt );
app_state_t app_state(void);
bool        app_destroy(void);

void        app_show(int);

// events

#define on_glcontext_create on_glcontext_create
void on_glcontext_create(void *fn, void *arg);

#define on_frame_begin on_frame_begin
void on_frame_begin(void *fn, void *arg);

#define on_frame_end on_frame_end
void on_frame_end(void *fn, void *arg);

// ----------------------------------------------------------------------------

#define app_create_flags(...)  (CAST(app_create_flags_t) {__VA_ARGS__})
#define app_swap_flags(...)  (CAST(app_swap_flags_t) {__VA_ARGS__})
#define app_create(...) app_create(app_create_flags(.resizable = 1, .visible = 1, .debug = !V3_RETAIL, .silent = V3_RETAIL, ##__VA_ARGS__))
#define app_swap(...)   app_swap(app_swap_flags(.hz = 1, .depth = 1, .stencil = 0, /*.color = 0xFFFF00FF,*/ ##__VA_ARGS__))

#define app_width()  app_state().w
#define app_height() app_state().h

#elif CODE
#pragma once
#include "app(glfw3).h"

void on_frame_end(void *fn, void *arg) { hook(on_frame_end, fn, arg); }
void on_frame_begin(void *fn, void *arg) { hook(on_frame_begin, fn, arg); }
void on_glcontext_create(void *fn, void *arg) { hook(on_glcontext_create, fn, arg); }

bool app_destroy(void) { /* @todo */ return true; }

#endif
