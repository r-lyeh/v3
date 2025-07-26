#if CODE
#pragma once
#define GLAD_GL_IMPLEMENTATION

#ifndef __EMSCRIPTEN__ // RGFW_WASM
    void load_gl(void *arg) {
        if( gladLoaderLoadGL() <= 0 )
            die("Failed to initialize GL");
    }

    AUTORUN {
        #ifdef on_graphics_context_created
        on_graphics_context_created(load_gl, NULL);
        #endif
    }
#endif

#endif

#define GLAD_API_CALL extern

#ifndef __EMSCRIPTEN__
#include "3rd/glad.h"
#else
#include <GLES3/gl3.h>
#endif

// #define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
// #define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
// #define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
// #define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
