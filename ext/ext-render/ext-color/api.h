#if !CODE

typedef union color_t {
    struct { uint8_t r,g,b,a; };
    uint32_t bits;
} color_t;

unsigned color24(unsigned r, unsigned g, unsigned b);
unsigned color32(unsigned r, unsigned g, unsigned b, unsigned a);

#else
#pragma once

unsigned color24(unsigned r, unsigned g, unsigned b) {
    return ((255<<24)|((b)<<16)|((g)<<8)|((r)<<0));
}
unsigned color32(unsigned r, unsigned g, unsigned b, unsigned a) {
    return (((a)<<24)|((b)<<16)|((g)<<8)|((r)<<0));
}

#endif
