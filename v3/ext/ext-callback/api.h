#if !CODE

typedef struct callback_t {
    void (*func1)(void*); void *arg;
    void (*func0)(void);
} callback_t;

#define callback_t(...) (ifdef(V3_C,(callback_t),callback_t) { __VA_ARGS__ })

extern void (*call)(callback_t);

#else
#pragma once
#include v3_callback_backend

void (*call)(callback_t) = call_;
#endif
