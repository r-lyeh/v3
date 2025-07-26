#ifndef THREAD_H
#define THREAD_H

#if V3_MACOS
#define thread_t thread_t2 // conflicts with mach/mach_types.h
#endif

typedef struct thread_t {
    void *vt;
    void (*func0)(void);
    void (*func1)(void *arg); void *arg;
    void *opaque;
} thread_t;

#define thread(...) thread(make(thread_t, ##__VA_ARGS__))

extern void     (*thread_t_vt[])();
extern thread_t (*thread)(thread_t);
extern void     (*thread_join)(thread_t*);
extern bool     (*thread_running)(thread_t*);

#elif CODE
#pragma once
#include v3_thread_backend

thread_t (*thread)(thread_t) = thread_;
void     (*thread_join)(thread_t*) = thread_join_;
bool     (*thread_running)(thread_t*) = thread_running_;

void (*thread_t_vt[])() = {
    ['D'] = thread_drop_,
};

#endif
