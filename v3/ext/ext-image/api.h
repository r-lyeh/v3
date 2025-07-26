#ifndef IMAGE_H
#define IMAGE_H

typedef struct image {
    void *vt;
    ifndef(retail, char debug[64];)
    union {
        void *data;
        uint8_t *data8;
        uint16_t *data16;
        uint32_t *data32;
        float *dataf;
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
        struct { int ch; };
        struct { int channels; };
    };
    union {
        struct { int flt; };
        struct { int isfloat; };
    };
    const char *url;
    const void *bin; int len;
} image_t;

#define image(...) image(init(image_t, ##__VA_ARGS__))

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
