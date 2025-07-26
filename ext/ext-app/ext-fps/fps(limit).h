//-----------------------------------------------------------------------------
// fps locking
// assumes window and rendering context are created with no VSYNC interval (~max fps)

#pragma comment(lib, "winmm")

static volatile float target_fps; // -1: use monitor refresh rate, 0: vsync off, 1: vsync on, N: specific fps
static volatile unsigned timer_counter, loop_counter;
static
void fps_thread_(void) {
    int64_t ns_excess = 0;
    while( thread_running(NULL) ) {
        if( target_fps <= 0 ) {
            loop_counter = timer_counter = 0;
            sleep_ms(250);
        } else {
            timer_counter++;
            int64_t tt = (int64_t)(1e9/(float)target_fps) - ns_excess;
            uint64_t took = -time_ns();
            #if V3_WINDOWS
            timeBeginPeriod(1);
            #endif
            sleep_ns( tt > 0 ? (float)tt : 0.f );
            took += time_ns();
            ns_excess = took - tt;
            if( ns_excess < 0 ) ns_excess = 0;
            //puts( va("%lld", ns_excess) );
        }
    }
}
static
void fps_locker( int on ) {
    static thread_t t;
    if( on ) {
        // private threaded timer
        timer_counter = loop_counter = 0;
        t = thread( .func0 = fps_thread_ );
    } else {
        thread_join(&t);
    }
}
// function that locks render to desired `target_fps` target_fps (in FPS).
// - returns true if must render, else 0.
static
int fps_wait0(void) {
    ONCE if( target_fps < 0 ) {
        // cap to monitor Hz, or user-requested Hz
        float maxfps = monitor_hz(0);
        float asked = flagi("--fps", (int)(maxfps+0.5)+2); /* 0 for immediate updates, 1 for updates synchronized with the vertical retrace, -1 for adaptive vsync, N to any given fps number. defaults to monitor Hz */
        // /**/ if( asked ==  0 ) target_fps = 0, SDL_GL_SetSwapInterval( 0); /* disable our fps limiter + vsync off: max speed */
        // else if( asked ==  1 ) target_fps = 0, SDL_GL_SetSwapInterval( 1); /* disable our fps limiter + vsync on : vsync caps fps to monitor hz */
        // else if( asked  <  0 ) target_fps = -maxfps, SDL_GL_SetSwapInterval(-1); /* disable our fps limiter + adaptive : vsync when above monitor hz and off when it's below */
        // else                   target_fps = asked; /* target specific target_fps */
        target_fps = asked;
    }

    if( target_fps <= 0 ) return 1; // @todo: vsync(ON) if == 1

    // kick-off timing thread
    ONCE fps_locker(1);

    // if we throttled too much, cpu idle wait
    while( (loop_counter > timer_counter) ) {
        //thread_yield();
        sleep_ns(100);
    }

    // max auto frameskip is 10: ie, even if speed is low paint at least one frame every 10
    enum { maxframeskip = 10 };
    if( timer_counter > loop_counter + maxframeskip ) {
        loop_counter = timer_counter;
    }
    loop_counter++;

    // only draw if we are fast enough, otherwise skip the frame
    return loop_counter >= timer_counter;
}

static void fps_wait1(void *arg) {
    fps_wait0();
}

AUTORUN {
    on_frame_begin(fps_wait1, NULL);
}
