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
