#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    image_t a = image(); // create 1x1 rgba image
    image_t b = image(.w = 320, .h = 240); // create rgba rect
    image_t c = image(.w = 320, .h = 240, .channels = 1); // create monochrome rect
    image_t d = image(.url = "brick.jpg"); // create image from file/url
    image_t e = image(.url = "brick.jpg", .w=100, .h=100); // create image from file/url, and resize to 100x100
    image_t f = image(.url = "brick.jpg", .w=0.50, .h=0.50); // create image from file/url, and downscale to 50%
#ifdef APP_H
    for( app_create(.title = "hi", "0.50"); app_swap(); ) {
        ui_image(a);
        ui_image(b);
        ui_image(c);
        ui_image(d);
        ui_image(e);
        ui_image(f);
    }
#endif
    image_drop(&a);
    image_drop(&b);
    image_drop(&c);
    image_drop(&d);
    image_drop(&e);
    image_drop(&f);
    return 0;
}
