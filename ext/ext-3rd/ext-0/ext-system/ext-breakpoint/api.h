extern void (*breakpoint)(void); // breaks-in if debugger is present

#if CODE
#pragma once
#include v3_breakpoint_backend

void (*breakpoint)(void) = breakpoint_;
#endif
