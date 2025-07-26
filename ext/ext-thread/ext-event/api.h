#if !CODE

void event(void *type);
void hook(void *type, void (*call)(void*), void *arg0);

#define hook(...) (printf("%.*s", 0, __func__), hook(__VA_ARGS__)) // dummy printf to prevent function aliasing in optimized builds. without this, all hooks could be redirected to a single event type because of optimizations (see: function folding + cl /O1)

#else
#pragma once

struct api_t {
    void *type;
    void (*call)(void*); void *arg0;
} api[64] = {0}; int apis;

void (hook)(void *type, void (*call)(void*), void *arg0) {
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
