image_t image_webp(image_t f);

#if CODE
#pragma once

//#define JEBP_IMPLEMENTATION
//#include "jebp.h"

//#define SIMPLEWEBP_IMPLEMENTATION
//#include "simplewebp.h"

//#include "whale.h"
//#include "whale.c"

/* #define twp__ENABLE_ASSERTS */
#define twp_IMPLEMENTATION
#include "3rd/tiny_webp.h"

image_t (*image_before_webp)(image_t);

image_t image_webp(image_t i) {
    if( i.url ) {
        ifndef(V3_RETAIL, strncpy(i.debug, i.url, COUNTOF(i.debug)));
        file_t f = asset(i.url);
        i.bin = f.bin, i.len = f.len;
    }
    if( i.bin && i.len ) {
  #if 0
        {
            jebp_image_t image;
            jebp_error_t err = jebp_decode(&image, i.len, i.bin);
            if (err == JEBP_OK) {
                assert(!"float colors not supported" && (i.floating == 0));
                assert(!"flipped images not supported" && (i.flip == 0));
                assert(!"color conversion not supported" && (i.ch == 0 || i.ch == 4));
                //assert(!"load from memory not supported" && (i.bin == 0 && i.len == 0));

                lputs(va("%p %d %d %d %d\n", image.pixels, image.width, image.height, 32, image.width * 4));

                image_t rsz = image_before_webp(make(image_t, .w = image.width, .h = image.height, .n = 4));
                memcpy(rsz.data, image.pixels, rsz.w * rsz.h * 4 );

                jebp_free_image(&image);
                return lputs(va("`%s` decoded with jebp", i.url ? i.url : "")), rsz;
            }
            lputs(va("jebp: %s", jebp_error_string(err)));
        }

        {
            simplewebp *swebp;
            simplewebp_error err = simplewebp_load_from_memory((void*)i.bin, i.len, NULL, &swebp);
            if( !err ) {
                size_t width, height;
                simplewebp_get_dimensions(swebp, &width, &height);
                image_t rsz = image_before_webp(make(image_t, .w = width, .h = height, .n = 4));
                err = simplewebp_decode(swebp, (void*)rsz.data, NULL);
                simplewebp_unload(swebp);
                if(!err) {
                    return lputs(va("`%s` decoded with swebp", i.url ? i.url : "")), rsz;
                }
                drop(&rsz);
            }
            lputs(va("swebp: %s", simplewebp_get_error_text(err)));
        }
#else
        {
            int width, height;
            unsigned char *data = twp_read_from_memory((void*)i.bin, i.len,
                                           &width, &height,
                                           i.n == 3 ? twp_FORMAT_RGB : twp_FORMAT_RGBA,
                                           0); // no flags
            if (data) {
                image_t dst = image_before_webp(CAST(image_t) { .vt = 0, .w = width, .h = height, .n = i.n, .flip = i.flip });
                if( i.flip ) for( int y = 0, e = dst.h - 1, w = dst.w * i.n; y <= e; ++y )
                memcpy(&dst.pixel8[0+(e-y)*w], &data[0+y*w], w);
                else
                memcpy(dst.data, data, dst.w * dst.h * i.n);

                free(data);

                ifndef(V3_RETAIL, strncpy(dst.debug, i.debug, COUNTOF(dst.debug)));
                return lputs(va("`%s` decoded with twp", i.url ? i.url : "")), dst;
            }
        }
#endif

    }
    lputs(va("`%s` decoding with stbi...", i.url ? i.url : ""));
    return image_before_webp(i);
}

AUTORUN {
    // replace image() with newer handler
    image_before_webp = image;
    image = image_webp;
}

#endif
