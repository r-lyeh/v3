#if !CODE

void hook(void *type, void (*call)(void*), void *arg0);
void event(void *type);

#else
#pragma once

struct api_t {
    void *type;
    void (*call)(void*); void *arg0;
} api[64] = {0}; int apis;

void hook(void *type, void (*call)(void*), void *arg0) {
    assert( apis < COUNTOF(api) );
    api[apis  ].type = type;
    api[apis  ].call = call;
    api[apis++].arg0 = arg0;
}
void event(void *type) {
    for( int i = 0; i < apis; ++i ) {
        if( api[i].type == type ) {
            api[i].call(api[i].arg0);
        }
    }
}

#endif
