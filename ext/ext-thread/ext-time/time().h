#define TIMER_E3 1000ULL
#define TIMER_E6 1000000ULL
#define TIMER_E9 1000000000ULL

#ifdef CLOCK_MONOTONIC_RAW
#define TIME_MONOTONIC CLOCK_MONOTONIC_RAW
#elif defined CLOCK_MONOTONIC
#define TIME_MONOTONIC CLOCK_MONOTONIC
#else
// #define TIME_MONOTONIC CLOCK_REALTIME // untested
#endif

static uint64_t nanotimer(uint64_t *out_freq) {
    if( out_freq ) {
#ifdef _WIN32
        LARGE_INTEGER li;
        QueryPerformanceFrequency(&li);
        *out_freq = li.QuadPart;
//#elif V3_ANDROID
//      *out_freq = CLOCKS_PER_SEC;
#elif defined TIME_MONOTONIC
        *out_freq = TIMER_E9;
#else
        *out_freq = TIMER_E6;
#endif
    }
#ifdef _WIN32
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (uint64_t)li.QuadPart;
//#elif V3_ANDROID
//    return (uint64_t)clock();
#elif defined TIME_MONOTONIC
    struct timespec ts;
    clock_gettime(TIME_MONOTONIC, &ts);
    return (TIMER_E9 * (uint64_t)ts.tv_sec) + ts.tv_nsec;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (TIMER_E6 * (uint64_t)tv.tv_sec) + tv.tv_usec;
#endif
}



uint64_t date_(void) {
    time_t epoch = time(0);
    struct tm *ti = localtime(&epoch);
    return atoi64_(va("%04d%02d%02d%02d%02d%02d",ti->tm_year+1900,ti->tm_mon+1,ti->tm_mday,ti->tm_hour,ti->tm_min,ti->tm_sec));
}
char *date_string_(void) {
    time_t epoch = time(0);
    struct tm *ti = localtime(&epoch);
    return va("%04d-%02d-%02d %02d:%02d:%02d",ti->tm_year+1900,ti->tm_mon+1,ti->tm_mday,ti->tm_hour,ti->tm_min,ti->tm_sec);
}
uint64_t date_epoch_(void) {
    time_t epoch = time(0);
    return epoch;
}

// high-perf functions

uint64_t time_ns_(void) {
    static threadlocal_ uint64_t epoch = 0;
    static threadlocal_ uint64_t freq = 0;
    if( !freq ) {
        epoch = nanotimer(&freq);
    }

    uint64_t a = nanotimer(NULL) - epoch;
    uint64_t b = TIMER_E9;
    uint64_t c = freq;

    // Computes (a*b)/c without overflow, as long as both (a*b) and the overall result fit into 64-bits.
    // [ref] https://github.com/rust-lang/rust/blob/3809bbf47c8557bd149b3e52ceb47434ca8378d5/src/libstd/sys_common/mod.rs#L124
    uint64_t q = a / c;
    uint64_t r = a % c;
    return q * b + r * b / c;
}
uint64_t time_us_(void) {
    return time_ns_() / TIMER_E3;
}
uint64_t time_ms_(void) {
    return time_ns_() / TIMER_E6;
}
double time_ss_(void) {
    return time_ns_() / 1e9; // TIMER_E9;
}
double time_mm_(void) {
    return time_ss_() / 60;
}
double time_hh_(void) {
    return time_mm_() / 60;
}

void sleep_ns_( double ns ) {
#ifdef _WIN32
    if( ns >= 100 ) {
        LARGE_INTEGER li;      // Windows sleep in 100ns units
        HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
        li.QuadPart = (LONGLONG)(__int64)(-ns/100); // Negative for relative time
        SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE);
        WaitForSingleObject(timer, INFINITE);
        CloseHandle(timer);
#else
    if( ns > 0 ) {
        struct timespec wait = {0};
        wait.tv_sec = ns / 1e9;
        wait.tv_nsec = ns - wait.tv_sec * 1e9;
        nanosleep(&wait, NULL);
#endif
    } else {
#ifdef _WIN32
        Sleep(0); // yield, Sleep(0), SwitchToThread
#else
        usleep(0);
#endif
    }
}
void sleep_us_( double us ) {
    sleep_ns_(us * 1e3);
}
void sleep_ms_( double ms ) {
    sleep_ns_(ms * 1e6);
}
void sleep_ss_( double ss ) {
    sleep_ns_(ss * 1e9);
}

void yield_(void) {
#ifdef _WIN32
    SwitchToThread(); // Sleep(0);
#else
    usleep(0);
#endif
}
