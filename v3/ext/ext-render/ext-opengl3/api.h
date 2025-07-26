#if CODE
#pragma once
#define GLAD_GL_IMPLEMENTATION

const char *GL_hints(void) {
#ifdef GL2
    return "";
#else
    return "GL33+"; // 3.30 core
#endif
}
bool GL_load(void *loader) {
#ifdef GL2
    return false;
#else
    return gladLoaderLoadGL() > 0;
    return !!gladLoadGL((GLADloadfunc)loader);
#endif
}

#endif

#define GLAD_API_CALL API

#ifdef _WIN32
#define OEMRESOURCE
#include <winsock2.h> // OCR_xxx defines
#endif

#ifdef GL2
#include <GL/gl.h>
#else
#ifndef __EMSCRIPTEN__
#include "glad.h"
#else
#include <GLES3/gl3.h>
#endif
#endif

// #define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
// #define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
// #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
// #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

// #define GL_DEBUG_SEVERITY_HIGH            0x9146
// #define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
// #define GL_DEBUG_SOURCE_API               0x8246
// #define GL_DEBUG_TYPE_ERROR               0x824C
