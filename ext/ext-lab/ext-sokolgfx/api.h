#if CODE
#pragma once
#define PICO_GFX_IMPLEMENTATION
#define SOKOL_GL_IMPL
#endif

#ifndef PICO_GFX_GL
#define PICO_GFX_GL
#endif

#ifndef SOKOL_EXTERNAL_GL_LOADER
#define SOKOL_EXTERNAL_GL_LOADER
#endif

#include "3rd/pico_gfx.h"
#include "3rd/sokol_gl.h"
#include "glue.h"
