#ifndef MOUSE_H
#define MOUSE_H

enum {
    mouse_idle, mouse_repeat, mouse_down, mouse_up,
};

extern int    (*mouse)(int vk); // 'x','y','w'heel, 'u'/'v' for deltax/deltay, 'l'eft,'m'iddle,'r'ight [also 0,1,2]
extern int    (*mouse_cursor)(int mode); // -1(getter),0(hide),1(arrow),2(hand),3(ibeam),4(cross),5(resize),6(deny)

#elif CODE
#pragma once
#include v3_mouse_backend

int (*mouse)(int vk) = mouse_;
int (*mouse_cursor)(int mode) = mouse_cursor_;

#endif
