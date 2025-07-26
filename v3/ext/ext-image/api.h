#ifndef IMAGE_H
#define IMAGE_H

typedef struct image_t {
    int w, h, c;
    void *pixels;
} image_t;

image_t (*image)( const void *file_or_buffer, unsigned len, unsigned num_channels );
void (*image_free)(image_t *);

#elif defined CODE
#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

image_t image_( const void *file_or_buffer, unsigned len, unsigned num_channels ) {
    image_t i = {0};

    if( !len ) i.pixels = stbi_load((const char*)file_or_buffer, &i.w, &i.h, &i.c, num_channels);
    else i.pixels = stbi_load_from_memory(file_or_buffer, len, &i.w, &i.h, &i.c, num_channels);
    i.c = num_channels;

    return i;
}
void image_free_(image_t *i) {
    if( i && i->pixels )
        stbi_image_free(i->pixels), i->pixels = 0;
}

image_t (*image)( const void *, unsigned, unsigned ) = image_;
void (*image_free)( image_t * ) = image_free_;

#endif
