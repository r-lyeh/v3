#ifndef V3_PROFILER
#define V3_PROFILER !V3_RETAIL
#endif

#ifndef V3_PROFILER_INFO
#define V3_PROFILER_INFO(...) __VA_ARGS__ "@" ifdef(V3_GCC, __FILE__, __FUNCTION__) ":" STRINGIZE(__LINE__)
#endif

#include "profiler_cpu.h"
#include "profiler_gpu.h"

#if !V3_PROFILER
#   undef  CPU_PROFILE
#   define CPU_PROFILE(section) for(int MACRO(i) = 1; MACRO(i); MACRO(i) = 0)
#   undef  GPU_PROFILE
#   define GPU_PROFILE(section) for(int MACRO(i) = 1; MACRO(i); MACRO(i) = 0)
#endif
