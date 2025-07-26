extern void (*die)(const char *msg);

#if CODE
#pragma once
#include v3_die_backend

void (*die)(const char *msg) = die_;
#endif
