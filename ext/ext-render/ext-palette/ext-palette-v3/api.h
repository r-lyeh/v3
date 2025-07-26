#if !CODE

enum V3_BRGO_PALETTE { // BRGO 4x4 palette. rlyeh, CC0 licensed.
    #define  P(P) ((255<<24)|(((P)&255)<<16)|((((P)>>8)&255)<<8)|((P)>>16))
    V3_BLACK=P(0x000000),V3_GRAY1 =P(0x505050),V3_GRAY2=P(0xB0B0B0),V3_WHITE =P(0xFFF5CC), // GRAYSCALE
    V3_NAVY =P(0x243777),V3_PURPLE=P(0x89008C),V3_TEAL =P(0x008750),V3_BROWN =P(0x801E00), // BRGO DARK
    V3_BLUE =P(0x1E00C8),V3_RED   =P(0xCC0000),V3_GREEN=P(0x00CC00),V3_ORANGE=P(0xFF8000), // BRGO NORMAL
    V3_CYAN =P(0x0080FF),V3_PINK  =P(0xFF0050),V3_AQUA =P(0x00F084),V3_YELLOW=P(0xFFE600), // BRGO BRIGHT
    #undef   P
};

#else
#pragma once

AUTORUN {
    unsigned default_palette[] = {
        V3_BLACK,V3_GRAY1 ,V3_GRAY2,V3_WHITE ,
        V3_NAVY ,V3_PURPLE,V3_TEAL ,V3_BROWN ,
        V3_BLUE ,V3_RED   ,V3_GREEN,V3_ORANGE,
        V3_CYAN ,V3_PINK  ,V3_AQUA ,V3_YELLOW,
    };
    palette_load(COUNTOF(default_palette), default_palette);
}

#endif
