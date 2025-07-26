#ifndef MATH_H
#define MATH_H

typedef union vec2 {
    struct { float V[2]; };
    struct { float x, y; };
    struct { float r, g; };
    struct { float u, v; };
    struct { float min, max; };
    struct { int ix, iy; };
} vec2, vec2_t;

typedef union vec3 {
    struct { float V[3]; };
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float u, v, s; };
    struct { float min, max, step; };
    struct { int ix, iy, iz; };
    struct { vec2 xy; float z_; };
} vec3, vec3_t;

typedef union vec4 {
    struct { float V[4]; };
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { float u, v, s, t; };
    struct { float min, max, step, u_; };
    struct { int ix, iy, iz, iw; };
    struct { vec3 xyz; float z_; };
} vec4, vec4_t;

#define vec2(...) ( ifdef(V3_C,(vec2),vec2) { __VA_ARGS__ } )
#define vec3(...) ( ifdef(V3_C,(vec3),vec3) { __VA_ARGS__ } )
#define vec4(...) ( ifdef(V3_C,(vec4),vec4) { __VA_ARGS__ } )

static inline double clamp(double v, double mind, double maxd) { return v <= mind ? mind : v >= maxd ? maxd : v; }
static inline vec3 lerp3(const vec3 a, const vec3 b, float u) {
    float m = 1 - u;
    return vec3(a.x*m+b.x*u,a.y*m+b.y*u,a.z*m+b.z*u);
}

#elif CODE
#pragma once
#include v3_math_backend

#endif
