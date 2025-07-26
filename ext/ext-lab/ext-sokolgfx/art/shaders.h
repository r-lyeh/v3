#ifndef SOKOL_SHDC_IMPL
#define SOKOL_SHDC_IMPL
#define vs_source_glsl410 vs_source_glsl410_A
#define fs_source_glsl410 fs_source_glsl410_A
#include "sprite_shader.h"
#undef  fs_source_glsl410
#undef  vs_source_glsl410
#define vs_source_glsl410 vs_source_glsl410_B
#define fs_source_glsl410 fs_source_glsl410_B
#include "particle_shader.h"
#undef  fs_source_glsl410
#undef  vs_source_glsl410
#endif
