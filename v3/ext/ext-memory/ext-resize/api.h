// allocator that works pretty much like realloc and dies when running out of memory. to sum up:
// - null + len = malloc
// - ptr  + len = resize
// - ptr  +  ~0 = get printable pointer info; or size info?
// - ptr  +   0 = free
// - null +   0 = ignore
// - null +  ~0 = get printable allocator info

// you could say you can stretch a NULL pointer to a given size to allocate,
// and resize any other allocated pointer to any other size, or to 0 to deallocate it.

// additionally, all these pointers are monitored for memory leaks automatically.

extern void* (*resize)(void *, size_t);

#if CODE
#pragma once
#include v3_resize_backend

void* (*resize)(void *, size_t) = resize_;
#endif

