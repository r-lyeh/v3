#ifndef PROFILER_GPU_H
#define PROFILER_GPU_H

struct gpu_profiler {
    GLuint query_id;
    GLboolean is_active;
    GLuint64 elapsed_time_ns;
};

void     gpu_profile_init(struct gpu_profiler* p);
void      gpu_profile_begin(struct gpu_profiler* p);
GLuint64  gpu_profile_end(struct gpu_profiler* p);
void     gpu_profile_destroy(struct gpu_profiler* p);

#define GPU_PROFILE(...) \
    static struct gpu_profiler MACRO(profiler), *MACRO(init) = 0; if(!MACRO(init)) gpu_profile_init(MACRO(init) = &MACRO(profiler)); \
    for(int MACRO(L) = (gpu_profile_begin(&MACRO(profiler)), 1); MACRO(L); MACRO(L) = (cpu_profile(gpu_profile_end(&MACRO(profiler)), V3_PROFILER_INFO("gpu " __VA_ARGS__)), 0) )

#elif CODE
#pragma once

void gpu_profile_init(struct gpu_profiler* p) {
    glGenQueries(1, &p->query_id);
    p->is_active = GL_FALSE;
    p->elapsed_time_ns = 0;
}

void gpu_profile_begin(struct gpu_profiler* p) {
    if (p->is_active) {
        printf("Profiler already active!\n");
        return;
    }
    glBeginQuery(GL_TIME_ELAPSED, p->query_id);
    p->is_active = GL_TRUE;
}

GLuint64 gpu_profile_end(struct gpu_profiler* p) { // @todo: add non-blocking version
    if (!p->is_active) {
        printf("Profiler not active!\n");
        return 0;
    }
    glEndQuery(GL_TIME_ELAPSED);
    p->is_active = GL_FALSE;
    
    // Wait for result to be available (blocking)
    GLint available = 0;
    while (!available) {
        glGetQueryObjectiv(p->query_id, GL_QUERY_RESULT_AVAILABLE, &available);
    }
    
    glGetQueryObjectui64v(p->query_id, GL_QUERY_RESULT, &p->elapsed_time_ns);
    return p->elapsed_time_ns;
}

void gpu_profile_destroy(struct gpu_profiler* p) {
    glDeleteQueries(1, &p->query_id);
}

#endif
