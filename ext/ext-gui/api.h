extern int (*ui_window)(const char *label, unsigned flags);
extern int (* ui_tree)(const char *label);
extern int (*  ui_label)(const char *txt);
extern int (*  ui_button)(const char *txt);
extern int (*  ui_buttons)(int num, /* const char* */...);
extern int (*  ui_section)(const char *txt, int open);
extern int (*  ui_float)(const char *txt, float *f, float minf, float maxf);
extern int (*  ui_bool)(const char *txt, void *b);
extern int (*  ui_rgba8)(const char *txt, void *f);
extern int (* ui_tree_end)();
extern int (* ui_popup)(const char *label);
extern int (* ui_popup_end)();
extern int (*ui_window_end)();


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
int (* ui_tree_end)() = ui_tree_end_;
int (* ui_popup)(const char *label) = ui_popup_;
int (* ui_popup_end)() = ui_popup_end_;
int (*ui_window_end)() = ui_window_end_;
#endif
