#ifndef UI_H
#define UI_H

extern int (*ui_window)(const char *label, unsigned flags);
extern int (* ui_tree)(const char *label);
extern int (*  ui_label)(const char *txt);
extern int (*  ui_button)(const char *txt);
extern int (*  ui_buttons)(int num, /* const char* */...);
extern int (*  ui_section)(const char *txt, int open);
extern int (*  ui_int)(const char *txt, int *i, int lo, int hi);
extern int (*  ui_float)(const char *txt, float *f, float lo, float hi);
extern int (*  ui_bool)(const char *txt, void *b);
extern int (*  ui_rgba8)(const char *txt, void *f);
extern int (*  ui_texture)(void *handle);
extern int (*  ui_label3)(const char *txt, float v[3]);
extern int (*  ui_label4)(const char *txt, float v[4]);
extern int (*  ui_vec2)(const char *txt, float v[2], float lo, float hi);
extern int (*  ui_vec3)(const char *txt, float v[3], float lo, float hi);
extern int (*  ui_vec4)(const char *txt, float v[4], float lo, float hi);
extern int (* ui_tree_end)(void);
extern int (* ui_popup)(const char *label);
extern int (* ui_popup_end)(void);
extern int (*ui_window_end)(void);

extern int (*ui_hovered)(void);

#elif CODE
#pragma once
#include v3_gui_backend

int (*ui_window)(const char *label, unsigned flags) = ui_window_;
int (* ui_tree)(const char *label) = ui_tree_;
int (*  ui_label)(const char *txt) = ui_label_;
int (*  ui_button)(const char *txt) = ui_button_;
int (*  ui_buttons)(int num, ...) = ui_buttons_;
int (*  ui_section)(const char *txt, int open) = ui_section_;
int (*  ui_int)(const char *txt, int *f, int lo, int hi) = ui_int_;
int (*  ui_float)(const char *txt, float *f, float lo, float hi) = ui_float_;
int (*  ui_bool)(const char *txt, void *b) = ui_bool_;
int (*  ui_rgba8)(const char *txt, void *f) = ui_rgba8_;
int (* ui_tree_end)(void) = ui_tree_end_;
int (* ui_popup)(const char *label) = ui_popup_;
int (* ui_popup_end)(void) = ui_popup_end_;
int (*ui_window_end)(void) = ui_window_end_;

int (*ui_hovered)(void) = ui_hovered_;

int ui_label3_(const char *txt, float v[3]) {
    return ui_label(va("%s: %f %f %f", txt, v[0], v[1], v[2]));
}
int ui_label4_(const char *txt, float v[4]) {
    return ui_label(va("%s: %f %f %f %f", txt, v[0], v[1], v[2], v[3]));
}
int ui_vec2_(const char *txt, float v[2], float lo, float hi) {
    int mod = 0;
    mod |= ui_label(txt);
    mod |= ui_float("x", v+0, lo, hi);
    mod |= ui_float("y", v+1, lo, hi);
    return mod;
}
int ui_vec3_(const char *txt, float v[3], float lo, float hi) {
    int mod = 0;
    mod |= ui_label(txt);
    mod |= ui_float("x", v+0, lo, hi);
    mod |= ui_float("y", v+1, lo, hi);
    mod |= ui_float("z", v+2, lo, hi);
    return mod;
}
int ui_vec4_(const char *txt, float v[4], float lo, float hi) {
    int mod = 0;
    mod |= ui_label(txt);
    mod |= ui_float("x", v+0, lo, hi);
    mod |= ui_float("y", v+1, lo, hi);
    mod |= ui_float("z", v+2, lo, hi);
    mod |= ui_float("w", v+3, lo, hi);
    return mod;
}

int (*ui_label3)(const char *, float [3]) = ui_label3_;
int (*ui_label4)(const char *, float [4]) = ui_label4_;
int (*ui_vec2)(const char *, float [2], float, float) = ui_vec2_;
int (*ui_vec3)(const char *, float [3], float, float) = ui_vec3_;
int (*ui_vec4)(const char *, float [4], float, float) = ui_vec4_;

int (*ui_texture)(void *) = ui_texture_;

#endif
