extern void*    (*make)(unsigned bytes, void (*destructor)(void*));
extern bool     (*same)(void *obj, void (*destructor)(void*)); // returns true if `p` belongs to `destructor`; if so, `p` and `~()` are both of same class
extern void     (*drop)(void *obj);

#ifdef CODE
#pragma once
#include v3_object_backend

void *   (*make)(unsigned, void (*)(void*)) = make_;
bool     (*same)(void *, void (*)(void*)) = same_;
void     (*drop)(void *) = drop_;
#endif