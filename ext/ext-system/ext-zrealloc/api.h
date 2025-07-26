#ifndef ZREALLOC_PREREQUISITES
#define ZREALLOC_PREREQUISITES

// redirect all allocation code paths, so:
// - ensure we have a single code path for all allocations (we only use realloc; no malloc,calloc,free).
// - ensure we zero-clear all our allocs (zrealloc). especially important while enlarging buffers.

#include ifdef(V3_BSD, <malloc/malloc.h>, <malloc.h>) // freebsd: <malloc_np.h>
ifdef(V3_ANDROID, size_t dlmalloc_usable_size(void*));

static void*  (*sys_realloc)(void*, size_t) = realloc;
static size_t (*sys_msize)(void *) = ifdef(V3_BSD, (size_t (*)(void *))malloc_size, ifdef(V3_WINDOWS, _msize, ifdef(V3_ANDROID, dlmalloc_usable_size, malloc_usable_size)));

#define realloc      zrealloc

#endif


#ifndef ZREALLOC_H
#define ZREALLOC_H

void* zrealloc(void* p, size_t sz);
char* zstrdup(const char *s);

#elif CODE
#pragma once

void* zrealloc(void* p, size_t siz) {
    if( siz != ~0u ) {
        if( siz > 0 ) {
            size_t old = p ? sys_msize(p) : 0u;
            if( siz > old ) {
                size_t diff = siz - old;
                p = sys_realloc(p, siz);
                if( p ) {
                    memset((char*)p + siz - diff, 0, diff);
                }
                return p;
            }
        }
        return sys_realloc(p, siz);
    }
    return (void*)(uintptr_t)sys_msize(p);
}

char* zstrdup(const char *s) {
    int l = strlen(s);
    return memcpy(zrealloc(0,l+1),s,l+1);
}

#endif
