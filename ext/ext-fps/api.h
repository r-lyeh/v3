float fps(void);

#if CODE
#include "fps(limit).h"
#include v3_fpslimit_backend

float fps_;

float fps(void) {
    return fps_;
}

static void fps_count(void* arg) {
    static int counter = 0; ++counter;
    static uint64_t then = 0; uint64_t now = time_ns();
    const int updates_per_sec = 30;
    const double every = 1e9/updates_per_sec;
    if( (now-then) > every )
        then = now - ((now-then) - every),
        fps_ = updates_per_sec*counter, counter = 0;
}

AUTORUN {
    on_frame_begin(fps_count, NULL);
}

#endif
