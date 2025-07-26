#ifndef KEYBOARD_H
#define KEYBOARD_H

enum {
    key_idle, key_held, key_down, key_up,
};

extern int    (*key)(int vk);
extern int    (*keyrepeat)(int vk);
extern int*   (*keyboard)(void);

extern unsigned * (*keytext)(unsigned *count);

#define key_up(vk)   (key(vk) == key_up)
#define key_down(vk) (key(vk) == key_down)
#define key_held(vk) (key(vk) == key_held)
#define key_idle(vk) (key(vk) == key_idle)

#elif CODE
#pragma once
#include v3_keyboard_backend

int (*key)(int vk) = key_;
int (*keyrepeat)(int vk) = keyrepeat_;
int*(*keyboard)(void) = keyboard_;

unsigned *(*keytext)(unsigned *count) = keytext_;

#endif
