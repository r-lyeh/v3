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

#define  vec2(...) ( ifdef(V3_C, (vec2), vec2) { __VA_ARGS__ } )
#define  vec3(...) ( ifdef(V3_C, (vec3), vec3) { __VA_ARGS__ } )
#define  vec4(...) ( ifdef(V3_C, (vec4), vec4) { __VA_ARGS__ } )

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

#define ptr22(m22) ( (float*)&(m22) )
#define ptr33(m33) ( (float*)&(m33) )
#define ptr44(m44) ( (float*)&(m44) )

#endif
