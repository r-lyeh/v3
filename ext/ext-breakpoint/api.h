extern void (*breakpoint)(void);

#if CODE
#pragma once
#include v3_breakpoint_backend

void (*breakpoint)(void) = breakpoint_;
#endif
