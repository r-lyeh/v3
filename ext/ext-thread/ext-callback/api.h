#if !CODE

typedef struct callback_t {
    void (*func0)(void);
    void (*func1)(void*);
    void (*func2)(void*,void*);
    union { void *arg, *arg0; }; void *arg1; void *arg2;
} callback_t;

#define callback_t(...) (ifdef(V3_C,(callback_t),callback_t) { __VA_ARGS__ })

extern void (*call)(callback_t);

#else
#pragma once
#include v3_callback_backend

void (*call)(callback_t) = call_;
#endif
