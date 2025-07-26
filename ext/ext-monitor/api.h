extern int    (*monitors)(void); // returns number of monitors [N]
extern double (*monitor_hz)(int monitor); // use 0 for default monitor; [1..N] otherwise
extern double (*monitor_width)(int monitor); // use 0 for default monitor; [1..N] otherwise
extern double (*monitor_height)(int monitor); // use 0 for default monitor; [1..N] otherwise
extern void   (*monitor_workarea)(int monitor, int rect[4]); // use 0 for default monitor; [1..N] otherwise

#if CODE
#pragma once
#include v3_monitor_backend

int    (*monitors)(void) = monitors_;
double (*monitor_hz)(int) = monitor_hz_;
double (*monitor_width)(int) = monitor_width_;
double (*monitor_height)(int) = monitor_height_;
void   (*monitor_workarea)(int, int[4]) = monitor_workarea_;
#endif
