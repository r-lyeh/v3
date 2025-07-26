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
        struct { float w; };
        struct { float width; };
    };
    union {
        struct { float h; };
        struct { float height; };
    };
    union {
        struct { int n; };
        struct { int c; };
        struct { int ch; };
        struct { int channels; };
    };
    union {
        struct { int flt; };
        struct { int floating; };
    };
    union {
        int flip;
    };
    const char *url;
    const void *bin; int len;
} image_t;

#define image(...) image(make(image_t, ##__VA_ARGS__))

extern void    (*image_t_vt[])();
extern image_t (*image)( image_t );

extern int (*ui_image)(image_t);
extern int (*ui_images)(void);

#elif defined CODE
#pragma once
#include v3_image_backend

image_t (*image)( image_t ) = image_;

void (*image_t_vt[92])() = {
    ['D'] = image_drop_,
};

int (*ui_image)(image_t) = ui_image_;
int (*ui_images)(void) = ui_images_;

#endif
