#ifndef MATH_H
#define MATH_H

#define vec2 vec2_
#define vec3 vec3_
#define vec4 vec4_
#define mat22 mat22_
#define mat33 mat33_
#define mat44 mat44_

#include v3_math_backend

#undef  vec2
#undef  vec3
#undef  vec4
#undef  mat22
#undef  mat33
#undef  mat44

typedef vec2_t vec2;
typedef vec3_t vec3;
typedef vec4_t vec4;

typedef mat22_t mat22;
typedef mat33_t mat33;
typedef mat44_t mat44;

#define  vec2(...) ( ifdef(V3_C,( vec2), vec2) { __VA_ARGS__ } )
#define  vec3(...) ( ifdef(V3_C,( vec3), vec3) { __VA_ARGS__ } )
#define  vec4(...) ( ifdef(V3_C,( vec4), vec4) { __VA_ARGS__ } )

#define mat22(...) ( ifdef(V3_C,(mat22),mat22) { __VA_ARGS__ } )
#define mat33(...) ( ifdef(V3_C,(mat33),mat33) { __VA_ARGS__ } )
#define mat44(...) ( ifdef(V3_C,(mat44),mat44) { __VA_ARGS__ } )

#define neg3(v3)            vec3_neg(v3)
#define add3(v3,w3)         vec3_add(v3,w3)
#define sub3(v3,w3)         vec3_sub(v3,w3)
#define scale3(v3,f)        vec3_mulf(v3,f)
#define cross3(v3,w3)       vec3_cross(v3,w3)
#define lerp3(v3,w3,alphaf) vec3_lerp(v3,w3,alphaf)
#define norm3(v3)           vec3_normalize(v3)
#define mul3(v3,w3)         vec3_mul(v3,w3)
#define dot3(v3,w3)         vec3_dot(v3,w3)
#define len3(v3)            vec3_length(v3)

static inline void basis3(vec3 *right, vec3 *fwd, vec3 n) {
    *right = (n.z*n.z) < (n.x*n.x) ? vec3(n.y,-n.x,0) : vec3(0,-n.z,n.y);
    *fwd = cross3(*right, n);
}

#define lookat44(pos3,eye3,up3)                           mat44_look_at_rh(pos3,eye3,up3)
#define perspective44(fov_radians_f,aspectf,nearf,farf)   mat44_perspective_fov_rh(fov_radians_f,aspectf,nearf,farf)

#define ptr22(m22) ( &(m22).x.x )
#define ptr33(m33) ( &(m33).x.x )
#define ptr44(m44) ( &(m44).x.x )

#if 0
typedef union vec2 {
    struct { float V[2]; };
    struct { float x, y; };
    struct { float r, g; };
    struct { float u, v; };
    struct { float min, max; };
    struct { int ix, iy; };
} vec2;

typedef union vec3 {
    struct { float V[3]; };
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float u, v, s; };
    struct { float min, max, step; };
    struct { int ix, iy, iz; };
    struct { vec2 xy; float z_; };
} vec3;

typedef union vec4 {
    struct { float V[4]; };
    struct { float x, y, z, w; };
    struct { float r, g, b, a; };
    struct { float u, v, s, t; };
    struct { float min, max, step, u_; };
    struct { int ix, iy, iz, iw; };
    struct { vec3 xyz; float z_; };
} vec4;

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


#endif
