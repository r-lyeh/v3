#ifndef EDITOR_APP_H
#define EDITOR_APP_H

typedef struct editor {
    struct editor *next; // void *singletons[5]; // play:// instance, game:// datas, settings://, cvs://, window[x]://

    char name[64], title[64], file[4096];
    int rect[4]; // x,y,w,h clip
    int tick_rate, draw_rate; // process every X*6 frames [0=off, 1=6Hz, 2=12Hz, 4=24Hz, 5=30Hz, 10=60Hz, 20=120Hz, 24=144Hz, 40=244Hz, 60=360Hz ...]
    int num_instances; // number of window instances (default: 1)
    int focus; // |1(keyboard), |2(mouse), |4(gamepad)
    int open, visible, deleted; // y/n

    // called only while game is paused:
    // void (*tick)(unsigned frame, float dt);
    // void (*draw)(unsigned frame, float dt, mat44 proj, mat44 view);
    // void (*send)(const char *whom, const char *msg);
    // void (*recv)(const char *msg);
    editor_events vt;

} editor, editor_t;

#define editor(...) (ifdef(V3_C,(editor),editor) { .name = __FILE__, .title = __FILE__, ##__VA_ARGS__ })

int     editor_push(editor vp); // from memory. returns handle
int     editor_load(const char *dllfile); // from file. returns handle
bool    editor_unload(int handle);

#elif CODE
#pragma once

static array_(editor) editors;

int editor_push(editor vp) {
    array_push(editors, vp);
    return array_count(editors) - 1;
}
int editor_load(const char *dllfile) { // @fixme
    editor z = {0};
    return editor_push(z);
}
bool editor_unload(int handle) {
    editors[handle] = ((editor) {0});
    editors[handle].deleted = 1;
    return 1;
}

// ---

void test_tick() {
    puts("editor is ticking");
}

AUTORUN {
    editor c = editor();
    c.vt.ON_TICK = test_tick;
    editor_push(c);
}

#endif
