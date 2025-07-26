#ifndef EDITOR_WINDOW_H
#define EDITOR_WINDOW_H

typedef struct editor_window {
    struct editor_window *next; // void *singletons[5]; // play:// instance, game:// datas, settings://, cvs://, window[x]://

    char name[64], title[64], file[4096];
    int rect[4]; // x,y,w,h clip
    int has_focus; // |1(keyboard), |2(mouse), |4(gamepad)
    int tick_rate, draw_rate; // process every X*6 frames [0=off, 1=6Hz, 2=12Hz, 4=24Hz, 5=30Hz, 10=60Hz, 20=120Hz, 24=144Hz, 40=244Hz, 60=360Hz ...]
    int num_instances; // number of window instances (default: 1)

    // called only while game is paused:
    void (*tick)(unsigned frame, float dt);
    void (*draw)(unsigned frame, float dt, mat44 proj, mat44 view);
    void (*send)(const char *whom, const char *msg);
    void (*recv)(const char *msg);

} editor_window, editor_window_t;

#define editor_window(...) (ifdef(V3_C,(editor_window_t),editor_window_t) { .name = __FILE__, .title = __FILE__, ##__VA_ARGS__ })

void editor_add_window(editor_window_t vp);
void editor_draw_windows(unsigned game_texture_id);

#elif CODE
#pragma once

static array_(editor_window_t) editor_windows;

void editor_add_window(editor_window_t vp) {
    array_push(editor_windows, vp);
}

void editor_draw_windows(unsigned game_texture_id) {
    for( int i = 0, e = array_count(editor_windows); i < e; ++i ) {
        if( ui_window(editor_windows[i].title, 0) ) {

            extern mu_Context *ui_ctx;
            mu_Container *win = mu_get_current_container(ui_ctx);
            mu_draw_image(ui_ctx, win->rect, mu_rect(0,0,128,128), (void*)(uintptr_t)game_texture_id);

            ui_window_end();
        }
    }
}

#endif
