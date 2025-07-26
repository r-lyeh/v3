int64_t cpu_profile(int64_t ns, const char *at);

#ifndef CPU_PROFILE
#define CPU_PROFILE(...) for( int64_t MACRO(L) = -time_ns(); MACRO(L) < 0; MACRO(L) = cpu_profile(time_ns()+MACRO(L), V3_PROFILER_INFO("cpu " __VA_ARGS__)) )
#endif

#if CODE
#pragma once

struct profile {
    int64_t ns;
    const char * at;
};

array_(struct profile) profiles;

int64_t cpu_profile(int64_t ns, const char *at) {
    array_push(profiles, (ifdef(V3_C,(struct profile),struct profile){ns, at}));
    return ns;
}

int profile_less(const void *a, const void *b) {
    const struct profile *p = (const struct profile *)a;
    const struct profile *q = (const struct profile *)b;
    return strcmp(p->at, q->at); // sort by title
    return q->at - p->at; // sort by time
}

void profiler_draw(void *arg) {
    int e = array_count(profiles);
    if(!e) return;

    qsort(profiles, e, sizeof(struct profile), profile_less);

    float ns, us, ms, ss;
    for( int i = 0; i < e; ++i )
        ns = profiles[i].ns,
        us = profiles[i].ns / 1e3,
        ms = profiles[i].ns / 1e6,
        ss = profiles[i].ns / 1e9,
        ui_float(profiles[i].at, &us, 0, 1e9);

    array_clear(profiles);
}

#if V3_PROFILER

AUTORUN {
    on_frame_end(profiler_draw, NULL);
}

#endif

#endif
