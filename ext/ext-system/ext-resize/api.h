// this allocator works pretty much like realloc() and dies when running out of memory.
// optionally, it can monitor allocations to detect memory leaks.
//
// ## about using realloc() for everything
// resize() follows realloc() convention. if that does not ring a bell,
// you could say you can resize() any allocated P pointer to any other N size, eg: resize(P,N)
// then also resize() those P pointers to 0 bytes to actually free them, eg: resize(P,0)
// from there, you can think you can enlarge a NULL pointer to any given size to do a malloc, eg: resize(NULL,N)
//
// to sum up:
// - resize(NULL, len) >> malloc
// - resize(ptr , len) >> realloc
// - resize(ptr ,   0) >> free
// - resize(NULL,   0) >> ignored
//
// special negative len as follows:
// - resize(ptr ,  ~0) >> get printable pointer summary info; @todo: summary content is implementation defined but first word is guaranteed to be number of allocated bytes for that pointer, so you can atoi64(that).
// - resize(NULL,  ~0) >> get printable allocator summary info; @todo: summary content is implementation defined but first word is guaranteed to be total memory consumption in bytes, so you can atoi64(that).

extern void* (*resize)(void *, size_t);

#if CODE
#pragma once
#include v3_resize_backend

void* (*resize)(void *, size_t) = resize_;
#endif
