unsigned palette(unsigned c);
unsigned palette_load(unsigned numcolors, unsigned *colors);
unsigned palette_loadf(const char *pathfile);

#if CODE
#pragma once

unsigned *pal, palcount;

unsigned palette(unsigned c) {
    return c < palcount ? pal[c] : c;
}
unsigned palette_load(unsigned numcolors, unsigned *colors) {
    if( colors && numcolors ) memcpy(pal = realloc(pal, 4*numcolors), colors, 4*(palcount = numcolors));
    return !!colors;
}
unsigned palette_loadf(const char *pathfile) {
    file_t f = asset(pathfile);
    return palette_load(f.len / 4, f.bin);
}
#endif
