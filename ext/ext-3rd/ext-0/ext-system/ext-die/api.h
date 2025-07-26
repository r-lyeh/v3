#ifndef DIE_H
#define DIE_H

typedef struct die_flags {
    int retcode, line;
    const char *file;
    const char *func;
    const char *title;
} die_flags, die_flags_t;

#define die_flags(...)  (ifdef(V3_C,(die_flags_t),die_flags_t) { .retcode = 0, .line = __LINE__, .file = __FILE__, .func = __func__, .title = "Warning", __VA_ARGS__})
#define die(error, ...) die(error, die_flags(__VA_ARGS__))

extern void (*die)(const char *error, die_flags opt);

#elif CODE
#pragma once
#include "die().h"

void (*die)(const char *error, die_flags opt) = die_;

#endif
