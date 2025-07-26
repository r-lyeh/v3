extern void   (*alert2)(const char *message, const char *title); // prints callstack if msg[0]=='!'
extern void   (*alert)(const char *message); // prints callstack if msg[0]=='!'

#if CODE
#pragma once
#include v3_alert_backend

void   (*alert2)(const char *message, const char *title) = alert2_;
void   (*alert)(const char *message) = alert_;
#endif
