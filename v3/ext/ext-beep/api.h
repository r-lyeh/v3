extern void (*beep)(void);

#ifdef CODE
#pragma once
#include v3_beep_backend

void (*beep)(void) = beep_;
#endif
