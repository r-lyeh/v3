// input related devices like keyboard, mouse, gamepads...

extern int    (*key)(int vk);
extern int    (*mouse)(int vk); // 'x','y','w'heel, 'u'/'v' for deltax/deltay, 'l'eft,'m'iddle,'r'ight [also 0,1,2]
extern int    (*mouse_cursor)(int mode); // -1(getter),0(hide),1(arrow),2(hand),3(ibeam),4(cross),5(resize),6(deny)

#if CODE
#pragma once
#include v3_input_backend

int (*key)(int vk) = key_;
int (*mouse)(int vk) = mouse_;
int (*mouse_cursor)(int mode) = mouse_cursor_;

#endif
