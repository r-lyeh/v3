#ifndef ALERT_H
#define ALERT_H

typedef struct { // prints callstack if any string starts with '!'
    union {
    const char *title;
    const char *c, *caption;
    const char *a, *about;
    };
    union {
    const char *text;
    const char *body;
    const char *m, *msg, *message;
    };
} alert_flags, alert_flags_t;

#define alert(...) alert(ifdef(V3_C,(alert_flags),alert_flags){__VA_ARGS__})

extern void (*alert)(alert_flags_t);

#elif CODE
#pragma once
#include v3_alert_backend

void (*alert)(alert_flags_t) = alert_;
#endif
