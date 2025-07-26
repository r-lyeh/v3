V3_API double fps(void);           // return frames per second
V3_API double spf(void);           // return seconds per frame

V3_API void   vsync(float hz);     // config frequency: -1: use monitor refresh rate, 0: vsync off, 1: vsync on, N: specific fps
V3_API float  hz(void);            // return configured frequency

#if CODE
#include "fps(limit).h"
#include v3_fpslimit_backend

double fps_, spf_;

double fps(void) {
    return fps_;
}
double spf(void) {
    return spf_;
}
float hz(void) {
    return target_fps;
}
void vsync(float hz) {
    target_fps = hz;
}

static void fps_count(void* arg) {
    static int counter = 0; ++counter;
    static uint64_t then = 0; uint64_t now = time_ns(); int64_t delta = now - then;
    const double every = 1e9;
    if( delta > every ) {
        then = now - (delta - every),
        fps_ = counter, counter = 0;
        spf_ = (delta / 1e9) / (fps_ + !fps_);
    }
}

AUTORUN {
    on_frame_begin(fps_count, NULL);
}

#endif
