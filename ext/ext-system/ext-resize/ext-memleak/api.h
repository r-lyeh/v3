// dont call these api functions. memory allocators should invoke them.

extern void* (*watch)(void* ptr, size_t sz);
extern void* (*forget)(void* ptr);

#if CODE
#pragma once
#include v3_memleaks_backend

void* (*watch)(void* ptr, size_t sz) = watch_;
void* (*forget)(void* ptr) = forget_;
#endif
