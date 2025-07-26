// #include "3rd/minithread.h"

volatile              int g_running = 1;
volatile threadlocal_ int thd_running = 1;

static
void thread_close_all_(void) {
    g_running = 0;
}

typedef struct thread_pair {
    mt_thread handle;
    void (*func0)(void);
    void (*func1)(void *); void *arg;
    volatile int *running;
} thread_pair;

void* thread_adapter_(void *p) {
    thread_pair *t = (thread_pair*)p;
    thd_running = 1, t->running = &thd_running;
    event(on_thread_begin);
    if( g_running && t->func0 ) t->func0();
    if( g_running && t->func1 ) t->func1(t->arg);
    event(on_thread_end);
    return NULL;
}

thread_t thread_(thread_t i) {
    thread_t o = i;

    thread_pair *p = malloc(sizeof(thread_pair));
    p->func0 = o.func0;
    p->func1 = o.func1;
    p->arg = o.arg;
    o.opaque = p;
    
    if( mt_thread_create(&p->handle, thread_adapter_, p))
        die("cannot create thread");

    return o;
}

bool thread_running_(thread_t *t) {
    if(t) for(thread_pair *p = (thread_pair*)(t->opaque); !p->running; ) return 0;
    return g_running && thd_running;
}

void thread_join_(thread_t *t) {
    thread_pair *p = (thread_pair*)(t->opaque);
    if( t->opaque ) p->running[0] = 0, mt_thread_join(p->handle);
    if( t->opaque ) free(t->opaque), t->opaque = NULL;
}

void thread_drop_(thread_t *obj) {
    thread_join_(obj);
}

AUTORUN {
    atexit(thread_close_all_);
}
