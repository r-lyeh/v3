#ifndef IMAGE_H
#define IMAGE_H

typedef struct image_t {
    int w, h, n, c;
    void *data;
    ifndef(retail, char url[64];)
} image_t;

extern image_t* (*image)( file_t file, unsigned channels ); // ch=[0..4]; 0=auto. drop() after use

#elif defined CODE
#pragma once
#include v3_image_backend

image_t* (*image)( file_t, unsigned ) = image_;

#endif
