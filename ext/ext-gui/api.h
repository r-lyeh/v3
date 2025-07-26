extern int (*ui_window)(const char *label, unsigned flags);
extern int (* ui_tree)(const char *label);
extern int (*  ui_label)(const char *txt);
extern int (*  ui_button)(const char *txt);
extern int (*  ui_buttons)(int num, /* const char* */...);
extern int (*  ui_section)(const char *txt, int open);
extern int (*  ui_float)(const char *txt, float *f, float minf, float maxf);
extern int (*  ui_bool)(const char *txt, void *b);
extern int (*  ui_rgba8)(const char *txt, void *f);
extern int (*  ui_label3)(const char *txt, float v[3]);
extern int (*  ui_label4)(const char *txt, float v[4]);
extern int (*  ui_vec2)(const char *txt, float v[2], float minf, float maxf);
extern int (*  ui_vec3)(const char *txt, float v[3], float minf, float maxf);
extern int (*  ui_vec4)(const char *txt, float v[4], float minf, float maxf);
extern int (* ui_tree_end)(void);
extern int (* ui_popup)(const char *label);
extern int (* ui_popup_end)(void);
extern int (*ui_window_end)(void);

extern int (*ui_hovered)(void);

#if CODE
#pragma once
#include v3_gui_backend

int (*ui_window)(const char *label, unsigned flags) = ui_window_;
int (* ui_tree)(const char *label) = ui_tree_;
int (*  ui_label)(const char *txt) = ui_label_;
int (*  ui_button)(const char *txt) = ui_button_;
int (*  ui_buttons)(int num, ...) = ui_buttons_;
int (*  ui_section)(const char *txt, int open) = ui_section_;
int (*  ui_float)(const char *txt, float *f, float minf, float maxf) = ui_float_;
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
int ui_vec2_(const char *txt, float v[2], float minf, float maxf) {
    int mod = 0;
    mod |= ui_label(txt);
    mod |= ui_float("x", v+0, minf, maxf);
    mod |= ui_float("y", v+1, minf, maxf);
    return mod;
}
int ui_vec3_(const char *txt, float v[3], float minf, float maxf) {
    int mod = 0;
    mod |= ui_label(txt);
    mod |= ui_float("x", v+0, minf, maxf);
    mod |= ui_float("y", v+1, minf, maxf);
    mod |= ui_float("z", v+2, minf, maxf);
    return mod;
}
int ui_vec4_(const char *txt, float v[4], float minf, float maxf) {
    int mod = 0;
    mod |= ui_label(txt);
    mod |= ui_float("x", v+0, minf, maxf);
    mod |= ui_float("y", v+1, minf, maxf);
    mod |= ui_float("z", v+2, minf, maxf);
    mod |= ui_float("w", v+3, minf, maxf);
    return mod;
}

int (*ui_label3)(const char *, float [3]) = ui_label3_;
int (*ui_label4)(const char *, float [4]) = ui_label4_;
int (*ui_vec2)(const char *, float [2], float, float) = ui_vec2_;
int (*ui_vec3)(const char *, float [3], float, float) = ui_vec3_;
int (*ui_vec4)(const char *, float [4], float, float) = ui_vec4_;

#endif
