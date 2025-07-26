void (dd_plane)(vec3 p, vec3 n, dd_flags opt) {
    float scale = opt.scale;

    // if n is too similar to up vector, use right. else use up vector
    vec3 v1 = cross3(n, dot3(n, vec3(0,1,0)) > 0.8f ? vec3(1,0,0) : vec3(0,1,0));
    vec3 v2 = cross3(n, v1);

    // draw axis
    (dd_line)(p, add3(p,n), opt);
    (dd_line)(p, add3(p,v1), opt);
    (dd_line)(p, add3(p,v2), opt);

    // get plane coords
    v1 = scale3(v1, scale);
    v2 = scale3(v2, scale);
    vec3 p1 = add3(add3(p, v1), v2);
    vec3 p2 = add3(sub3(p, v1), v2);
    vec3 p3 = sub3(sub3(p, v1), v2);
    vec3 p4 = sub3(add3(p, v1), v2);

    // draw plane
    (dd_line)(p1, p2, opt);
    (dd_line)(p2, p3, opt);
    (dd_line)(p3, p4, opt);
    (dd_line)(p4, p1, opt);
}
