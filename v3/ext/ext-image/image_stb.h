#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void image_free_(void *p) {
    image_t *i = p;
    if( i->data ) {
        stbi_image_free(i->data);
    }
}

image_t* image_(file_t b, unsigned num_channels) {
    image_t *i = make(sizeof(image_t), image_free_);
    ifndef(retail, strncpy(i->url, b.url, COUNTOF(i->url)));

    i->data = b.bin && b.len ? stbi_load_from_memory(b.bin, b.len, &i->w, &i->h, &i->c, i->n = num_channels) : NULL;

    if(!i->data) {
        i->data = resize(0, num_channels);
        i->w = i->h = i->c = num_channels;
    }

    return i;
}
