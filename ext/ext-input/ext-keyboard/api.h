#ifndef KEYBOARD_H
#define KEYBOARD_H

enum {
    key_idle, key_repeat, key_down, key_up,
};

extern int    (*key)(int vk);
extern int    (*keyrepeat)(int vk);
extern int*   (*keyboard)(void);

extern unsigned * (*keytext)(unsigned *count);

#elif CODE
#pragma once
#include v3_keyboard_backend

int (*key)(int vk) = key_;
int (*keyrepeat)(int vk) = keyrepeat_;
int*(*keyboard)(void) = keyboard_;

unsigned *(*keytext)(unsigned *count) = keytext_;

#endif
