#ifndef IMAGE_H
#define IMAGE_H

typedef struct image {
    void *vt;
    ifndef(V3_RETAIL, char debug[64];)
    union {
        void *data, *bitmap, *pixel, *pixels, *v0;
        uint8_t *data8, *bitmap8, *pixel8, *u8, *r;
        uint16_t *data16, *bitmap16, *pixel16, *u16, *rg;
        uint32_t *data32, *bitmap32, *pixel32, *u32, *rgba;
        float *dataf, *bitmapf, *pixelf, *f32;
    };
    // flags
    union {
        float w, width;
    };
    union {
        float h, height;
    };
    union {
        int n, c, ch, channels;
    };
    union {
        int flt, floating;
    };
    union {
        int flip;
    };
    const char *url;
    const void *bin; int len;
} image_t;

#define image(...) image(CAST(image_t) { .vt = 0, ##__VA_ARGS__ })

extern image_t (*image)( image_t );
extern void    (*image_drop)( image_t* );

extern int (*ui_image)(image_t);
extern int (*ui_images)(void);

#elif defined CODE
#pragma once
#include v3_image_backend

image_t (*image)( image_t ) = image_;
void    (*image_drop)( image_t* ) = image_drop_;

int (*ui_image)(image_t) = ui_image_;
int (*ui_images)(void) = ui_images_;

#endif
