image_t image_pug(image_t f);

#if CODE
#pragma once

image_t (*image_loader_before_pug)(image_t);

image_t image_pug(image_t i) {
    if( i.url && !(i.bin && i.len) ) {
        ifndef(V3_RETAIL, strncpy(i.debug, i.url, COUNTOF(i.debug)));
        file_t f = file(i.url);
        i.bin = f.bin, i.len = f.len;
    }
    if( i.bin && i.len > 12 ) {
        unsigned *tag = (unsigned *)(((char*)i.bin) + i.len -  4);
        if( memcmp(tag, "pug1", 4) == 0 ) {
            unsigned jpglen = tag[-2], pnglen = tag[-1];

            image_t color = image_loader_before_pug(make(image_t,.flip=i.flip,.c=4,.len=jpglen,.bin=i.bin));
            image_t alpha = image_loader_before_pug(make(image_t,.flip=i.flip,.c=1,.len=pnglen,.bin=jpglen+((char*)i.bin)));

            for( int i = 0, e = alpha.w * alpha.h; i < e; ++i ) {
                color.rgba[i] = (alpha.r[i] << 24) | (color.rgba[i] & 0xFFFFFF);
            }

            ifndef(V3_RETAIL, strncpy(color.debug, i.url, COUNTOF(color.debug)));
            return drop(&alpha), color;
        }
    }
    return image_loader_before_pug(i);
}

AUTORUN {
    // replace image() with new handler
    image_loader_before_pug = image;
    image = image_pug;
}

#endif
