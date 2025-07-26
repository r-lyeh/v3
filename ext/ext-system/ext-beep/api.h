extern void (*beep)(void);

#if CODE
#pragma once
#include v3_beep_backend

void (*beep)(void) = beep_;
#endif
