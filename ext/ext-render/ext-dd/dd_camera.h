// original artwork by Alexandri Zavodny
// - rlyeh, public domain.

void (dd_camera)(vec3 position, dd_flags opt) {
    vec3 direction = opt.dir, upvector = opt.up;

    // Create transformation matrix
    mat44 transform = dd_make_transform(position, direction, upvector);

    // Draw cube (rectangle)
    vec3 p0 = transform_point(vec3( 0.5f,  0.5f,  1.0f));
    vec3 p1 = transform_point(vec3( 0.5f,  0.5f, -1.0f));
    vec3 p2 = transform_point(vec3(-0.5f,  0.5f, -1.0f));
    vec3 p3 = transform_point(vec3(-0.5f,  0.5f,  1.0f));
    vec3 p4 = transform_point(vec3( 0.5f, -0.5f,  1.0f));
    vec3 p5 = transform_point(vec3( 0.5f, -0.5f, -1.0f));
    vec3 p6 = transform_point(vec3(-0.5f, -0.5f, -1.0f));
    vec3 p7 = transform_point(vec3(-0.5f, -0.5f,  1.0f));

    (dd_line)(p0, p1, opt);
    (dd_line)(p1, p2, opt);
    (dd_line)(p2, p3, opt);
    (dd_line)(p3, p0, opt);

    (dd_line)(p4, p5, opt);
    (dd_line)(p5, p6, opt);
    (dd_line)(p6, p7, opt);
    (dd_line)(p7, p4, opt);

    (dd_line)(p0, p4, opt);
    (dd_line)(p1, p5, opt);
    (dd_line)(p2, p6, opt);
    (dd_line)(p3, p7, opt);

    // Draw two half-reels, both centered at y=0.5, spanning y=[0.0, 1.0]
    for(int reel = 0; reel < 2; reel++) {
        float r = 0.25f;
        int detail = 32;
        vec3 c = transform_point(vec3(0.0f, 0.5f, -0.5f + 1.0f * reel));
        vec3 s = c; // Start at center for half-reel
        vec3 from = s;
        for(int i = 0; i <= detail / 2; i++) { // Half-circle: 0 to π
            float theta = (float)i / (float)(detail / 2) * DD_PI;
            float ex = -cosf(theta);
            float why = sinf(theta);
            vec3 p = transform_point(vec3(0.0f, 0.5f + why * r * 2.0f, -0.5f + 1.0f * reel + 2.0f * ex * r));
            vec3 to = p;
            (dd_line)(from, to, opt);
            from = to;
        }
        vec3 f = transform_point(vec3(0.0f, 0.5f, -0.5f + 1.0f * reel));
        (dd_line)(from, f, opt);
    }

    // Lens shield
    float lensOut = 0.2f;
    float lensOutZ = 0.4f;
    float lensOff = 0.3f;
    vec3 v0 = transform_point(vec3( 0.5f,  0.5f,  1.0f));
    vec3 v1 = transform_point(vec3(-0.5f,  0.5f,  1.0f));
    vec3 v2 = transform_point(vec3(-0.5f, -0.5f,  1.0f));
    vec3 v3 = transform_point(vec3( 0.5f, -0.5f,  1.0f));

    vec3 l0 = transform_point(add3(vec3( 0.5f,  0.5f,  1.0f), vec3( lensOut,  lensOff,  lensOutZ)));
    vec3 l1 = transform_point(add3(vec3(-0.5f,  0.5f,  1.0f), vec3(-lensOut,  lensOff,  lensOutZ)));
    vec3 l2 = transform_point(add3(vec3(-0.5f, -0.5f,  1.0f), vec3(-lensOut, -lensOff,  lensOutZ)));
    vec3 l3 = transform_point(add3(vec3( 0.5f, -0.5f,  1.0f), vec3( lensOut, -lensOff,  lensOutZ)));

    (dd_line)(l0, l1, opt);
    (dd_line)(l1, l2, opt);
    (dd_line)(l2, l3, opt);
    (dd_line)(l3, l0, opt);

    (dd_line)(v0, l0, opt);
    (dd_line)(v1, l1, opt);
    (dd_line)(v2, l2, opt);
    (dd_line)(v3, l3, opt);
}
