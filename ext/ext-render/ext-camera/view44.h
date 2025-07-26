extern mat44 last_view;

mat44 view44(camera_t c); // builds view matrix

#if CODE
#pragma once

mat44 last_view;

mat44 view44(camera_t c) { // builds view matrix
    vec3 target = c.lookat ? *c.lookat : add3(c.pos, norm3(c.fwd));
    return last_view = lookat44(c.pos, target, norm3(c.up));
}

#endif
