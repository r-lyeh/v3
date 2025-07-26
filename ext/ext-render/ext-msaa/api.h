bool msaa_begin(int samples);
bool msaa_end(void);

#if CODE
#pragma once
#include "msaa_fbo.h"

static MSAA_FBO msaa;

bool msaa_begin(int samples) {
    if(!msaa.width) msaa_fbo_init(&msaa);
    msaa_fbo_bind(&msaa, msaa.level = samples);
    return 1;
}
bool msaa_end(void) {
    msaa_fbo_blit(&msaa);
    return 1;
}
#endif
