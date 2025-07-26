#ifndef PROJ44_H
#define PROJ44_H

struct proj44_flags {
    float aspect;
    float znear;
    float zfar;
    bool  zreverse;
};

mat44_t proj44(float fov_deg, struct proj44_flags opt); // builds projection matrix

#define proj44(fov_deg,...)  proj44(fov_deg, proj44_flags(__VA_ARGS__))
#define proj44_flags(...)   (CAST(struct proj44_flags){.aspect=0, .znear=0.25f, .zfar=1024, .zreverse=0, ##__VA_ARGS__ })

extern mat44 last_proj;

#elif CODE
#pragma once

mat44 last_proj;

mat44_t (proj44)(float fov_deg, struct proj44_flags opt) { // builds projection matrix
    // The depth buffer in OpenGL uses a nonlinear mapping where precision is highest near the near plane and degrades quadratically with distance. For a 24-bit depth buffer, there are 2^{24} = 16,777,216 distinct depth values available. The minimal resolvable depth difference δz at a distance z is approximately δz ≈ z² / (near × 2^{24}).
    // To infer an optimal far plane from the 24-bit limit, we set the far plane such that δz at z = far is equal to the near distance (i.e., the resolution at far matches the scale set by near, providing a balanced point where precision remains usable across the range without excessive z-fighting for typical scenes). This yields the equation far² / (near × 2^{24}) = near, which simplifies to far = near × 2^{12} = near × 4096.
    // This ratio (far/near = 4096) corresponds to losing roughly half the depth bits (12 bits) due to the far/near ratio, leaving 12 effective bits of precision at the far plane—a reasonable threshold inferred from the bit limit for many applications
    //
    // Near Distance   Optimal Far Distance
    // 1                    4096
    // 0.5                  2048
    // 0.25                 1024
    // 0.1                  409.6
    // 0.01                 40.96
    // 0.001                4.096

    float aspect = opt.aspect ? opt.aspect : (app_width() / (float)app_height());
    if( opt.zreverse ) {
        return last_proj = perspective44(vecmath_radians(fov_deg), aspect, opt.zfar, opt.znear);
    } else {
        return last_proj = perspective44(vecmath_radians(fov_deg), aspect, opt.znear, opt.zfar);
    }
}

#endif
