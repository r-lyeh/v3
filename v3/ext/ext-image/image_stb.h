#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#include "stb_image_resize2.h"

void image_drop_(void *p) {
    image_t *i = p;
    if( i->data ) {
        stbi_image_free(i->data);
    }
}

image_t image_(image_t f) {
    image_t i = f;

    if( i.url ) {
        ifndef(retail, strncpy(i.debug, f.url, COUNTOF(i.debug)));
        file_t x = file(i.url);
        i.bin = x.bin, i.len = x.len;
    }

    if( i.bin && i.len ) { // @todo: honor isfloat stbif
        int w, h, n;
        i.data = stbi_load_from_memory(i.bin, i.len, &w, &h, &n, i.channels);
        i.w = w, i.h = h;
        if(!i.channels) i.channels = n;
    }

    if(!i.data) {

        i.width = i.width > 0 ? i.width : 1;
        i.height = i.height > 0 ? i.height : 1;
        i.channels = i.channels > 0 ? i.channels : 4;
        i.isfloat = !!i.isfloat;

        i.data = STBI_MALLOC(4 + i.width * i.height * i.channels * (1+3*!!i.isfloat));

        // @todo: draw checkerboard
    
    } else {

        // resize image if valid WxH constrains are given
        if( f.w || f.h ) {
            f.w = f.w <= 0 ? i.w : f.w < 1 ? f.w * i.w : f.w;
            f.h = f.h <= 0 ? i.h : f.h < 1 ? f.h * i.h : f.h;

            int filtered = 0;
            int stride = i.channels * (1+3*!!i.isfloat);

            // STBIR_FILTER_DEFAULT      = 0,  // use same filter type that easy-to-use API chooses
            // STBIR_FILTER_BOX          = 1,  // A trapezoid w/1-pixel wide ramps, same result as box for integer scale ratios
            // STBIR_FILTER_TRIANGLE     = 2,  // On upsampling, produces same results as bilinear texture filtering
            // STBIR_FILTER_CUBICBSPLINE = 3,  // The cubic b-spline (aka Mitchell-Netrevalli with B=1,C=0), gaussian-esque
            // STBIR_FILTER_CATMULLROM   = 4,  // An interpolating cubic spline
            // STBIR_FILTER_MITCHELL     = 5,  // Mitchell-Netrevalli filter with B=1/3, C=1/3
            // STBIR_FILTER_POINT_SAMPLE = 6,  // Simple point sampling
            // STBIR_FILTER_OTHER        = 7,  // User callback specified
            // STBIR_DEFAULT_FILTER_UPSAMPLE = STBIR_FILTER_CATMULLROM
            // STBIR_DEFAULT_FILTER_DOWNSAMPLE = STBIR_FILTER_MITCHELL
            void *out =
            stbir_resize(   i.data, i.w, i.h, i.w * stride,
                            NULL, f.w, f.h, f.w * stride,
                            i.channels/*STBIR_RGBA*/, i.isfloat ? STBIR_TYPE_FLOAT : STBIR_TYPE_UINT8,
                            STBIR_EDGE_CLAMP, filtered ? STBIR_FILTER_MITCHELL : STBIR_FILTER_POINT_SAMPLE );

            if( out ) {
                stbi_image_free(i.data);
                i.data = out;

                i.w = f.w;
                i.h = f.h;
            }
        }
    }

    return i;
}

int ui_images_(void) { return 0; }
int ui_image_(image_t i) {
    ui_label(va("%dx%d (%.*s) %s", (int)i.w, (int)i.h, i.ch, "RGBA", ifdef(retail, "", i.debug)));
    return 1;
}
