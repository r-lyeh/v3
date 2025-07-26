void dd_bounds(const vec3 points[8], dd_flags opt) {
    for( int i = 0; i < 4; ++i ) {
        (dd_line)(points[i], points[(i + 1) & 3], opt);
        (dd_line)(points[i], points[4 + i], opt);
        (dd_line)(points[4 + i], points[4 + ((i + 1) & 3)], opt);
    }
}
void dd_bounds_corners(const vec3 points[8], dd_flags opt) {
    for( int i = 0; i < 4; ++i ) {
        #define dd_unit(a,b) (dd_line)(a,add3(a,norm3(sub3(b,a))),opt), (dd_line)(b,add3(b,norm3(sub3(a,b))), opt)
        dd_unit(points[i], points[(i + 1) & 3]);
        dd_unit(points[i], points[4 + i]);
        dd_unit(points[4 + i], points[4 + ((i + 1) & 3)]);
        #undef dd_unit
    }
}
void (dd_frustum)(mat44 proj, mat44 view, dd_flags opt) {
    mat44 projview = mat44_mul_mat44(view, proj);

    mat44 clipmatrix;
    if( !mat44_inverse(&clipmatrix, NULL, projview) ) return; // early return if clip matrix cannot be deduced

    // Start with the standard clip volume, then bring it back to world space.
    const vec3 planes[8] = {
        {-1,-1,-1}, {+1,-1,-1}, {+1,+1,-1}, {-1,+1,-1}, // near plane
        {-1,-1,+1}, {+1,-1,+1}, {+1,+1,+1}, {-1,+1,+1}, // far plane
    };

    vec3 points[8];
    float wCoords[8];

    // Transform the planes by the inverse clip matrix:
    for( int i = 0; i < 8; ++i ) {
        // wCoords[i] = matTransformPointXYZW2(&points[i], planes[i], clipmatrix);
        vec3 *out = &points[i], in = planes[i]; const float *m = &clipmatrix.x.x;
        out->x = (m[0] * in.x) + (m[4] * in.y) + (m[ 8] * in.z) + m[12]; // in.w (vec4) assumed to be 1
        out->y = (m[1] * in.x) + (m[5] * in.y) + (m[ 9] * in.z) + m[13];
        out->z = (m[2] * in.x) + (m[6] * in.y) + (m[10] * in.z) + m[14];
        wCoords[i] =  (m[3] * in.x) + (m[7] * in.y) + (m[11] * in.z) + m[15]; // rw

        // bail if any W ended up as zero.
        const float epsilon = 1e-9f;
        if (fabs(wCoords[i]) < epsilon) {
            return;
        }
    }

    // Divide by the W component of each:
    for( int i = 0; i < 8; ++i ) {
        points[i].x /= wCoords[i];
        points[i].y /= wCoords[i];
        points[i].z /= wCoords[i];
    }

    // Connect the dots:
    dd_bounds(points, opt);
}
