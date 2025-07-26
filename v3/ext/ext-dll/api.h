extern void* (*dll)(const char *filename, const char *symbol);

#if CODE
#pragma once
#include v3_dll_backend

void* (*dll)(const char *filename, const char *symbol) = dll_;
#endif
