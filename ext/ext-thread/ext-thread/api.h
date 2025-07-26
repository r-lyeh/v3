#ifndef THREAD_H
#define THREAD_H

#if V3_MACOS
#include <malloc/malloc.h> // malloc.h > mach/mach_types.h
#define thread_t thread_t2 // conflicts with mach/mach_types.h
#endif

typedef struct thread_t {
    void *vt;
    void (*func0)(void);
    void (*func1)(void *arg); void *arg;
    void *opaque;
} thread_t;

#define thread(...) thread(CAST(thread_t) { .vt = 0, ##__VA_ARGS__ })

extern thread_t (*thread)(thread_t);
extern void     (*thread_join)(thread_t*);
extern bool     (*thread_running)(thread_t*);
extern void     (*thread_drop)(thread_t*);

extern void     on_thread_begin(void *fn, void *arg);
extern void     on_thread_end(void *fn, void *arg);

#elif CODE
#pragma once
#include v3_thread_backend

thread_t (*thread)(thread_t) = thread_;
void     (*thread_join)(thread_t*) = thread_join_;
bool     (*thread_running)(thread_t*) = thread_running_;
void     (*thread_drop)(thread_t*) = thread_drop_;

void on_thread_begin(void *fn, void *arg) { hook(on_thread_begin, fn, arg); }
void on_thread_end(void *fn, void *arg) { hook(on_thread_end, fn, arg); }

#endif
