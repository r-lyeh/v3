/*
minithread - minimal cross platform threading

Copyright (c) Arne Koenig 2025
Redistribution and use in source and binary forms, with or without modification, are permitted.
THIS SOFTWARE IS PROVIDED 'AS-IS', WITHOUT ANY EXPRESS OR IMPLIED WARRANTY. IN NO EVENT WILL THE AUTHORS BE HELD LIABLE FOR ANY DAMAGES ARISING FROM THE USE OF THIS SOFTWARE.
*/

#pragma once

#ifdef _WIN32
#include <winsock2.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef HANDLE mt_thread;
typedef CRITICAL_SECTION mt_mutex;
typedef volatile LONG mt_atomic_int32;
#else
typedef pthread_t mt_thread;
typedef pthread_mutex_t mt_mutex;
typedef volatile int32_t mt_atomic_int32;
#endif

typedef void* (*mt_thread_func)(void*);


static inline int mt_thread_create(mt_thread* thread, mt_thread_func func, void* arg) {
#ifdef _WIN32
    DWORD thread_id;
    *thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, &thread_id);
    return *thread != NULL ? 0 : -1;
#else
    return pthread_create(thread, NULL, func, arg);
#endif
}

static inline int mt_thread_join(mt_thread thread) {
#ifdef _WIN32
    DWORD result = WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    return result == WAIT_OBJECT_0 ? 0 : -1;
#else
    return pthread_join(thread, NULL);
#endif
}

static inline void mt_thread_sleep_ms(unsigned int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}


static inline int mt_mutex_init(mt_mutex* mutex) {
#ifdef _WIN32
    InitializeCriticalSection(mutex);
    return 0;
#else
    return pthread_mutex_init(mutex, NULL);
#endif
}

static inline void mt_mutex_destroy(mt_mutex* mutex) {
#ifdef _WIN32
    DeleteCriticalSection(mutex);
#else
    pthread_mutex_destroy(mutex);
#endif
}

static inline void mt_mutex_lock(mt_mutex* mutex) {
#ifdef _WIN32
    EnterCriticalSection(mutex);
#else
    pthread_mutex_lock(mutex);
#endif
}

static inline void mt_mutex_unlock(mt_mutex* mutex) {
#ifdef _WIN32
    LeaveCriticalSection(mutex);
#else
    pthread_mutex_unlock(mutex);
#endif
}

static inline void mt_atomic_init(mt_atomic_int32* a, int32_t value) {
    *a = value;
}

static inline int32_t mt_atomic_load(const mt_atomic_int32* a) {
#ifdef _WIN32
    return InterlockedCompareExchange((volatile LONG*)a, 0, 0);
#elif defined(__clang__) || defined(__GNUC__)
    return __atomic_load_n(a, __ATOMIC_SEQ_CST);
#else
    return __sync_fetch_and_add((volatile int32_t*)a, 0);
#endif
}

static inline void mt_atomic_store(mt_atomic_int32* a, int32_t value) {
#ifdef _WIN32
    InterlockedExchange((volatile LONG*)a, value);
#elif defined(__clang__) || defined(__GNUC__)
    __atomic_store_n(a, value, __ATOMIC_SEQ_CST);
#else
    do {
        int32_t old = __sync_fetch_and_add(a, 0);
    } while (!__sync_bool_compare_and_swap(a, old, value));
#endif
}

static inline int32_t mt_atomic_increment(mt_atomic_int32* a) {
#ifdef _WIN32
    return InterlockedIncrement((volatile LONG*)a);
#else
    return __sync_add_and_fetch(a, 1);
#endif
}

static inline int32_t mt_atomic_decrement(mt_atomic_int32* a) {
#ifdef _WIN32
    return InterlockedDecrement((volatile LONG*)a);
#else
    return __sync_sub_and_fetch(a, 1);
#endif
}

static inline int32_t mt_atomic_add(mt_atomic_int32* a, int32_t x) {
#ifdef _WIN32
    return InterlockedAdd((volatile LONG*)a, x);
#else
    return __sync_add_and_fetch(a, x);
#endif
}

#ifdef __cplusplus
}
#endif
