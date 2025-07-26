void (dd_grid)(dd_flags opt) {
    float scale = opt.scale;
    if( scale ) {
        for( float i = -scale, c = 0; c <= 20; c += 20, i += c * (scale/10) ) {
            dd_line(vec3(-scale,0,i), vec3(+scale,0,i), .color = V3_GRAY2); // horiz
            dd_line(vec3(i,0,-scale), vec3(i,0,+scale), .color = V3_GRAY2); // vert
        }
        for( float i = -scale + scale/10, c = 1; c < 20; ++c, i += (scale/10) ) {
            dd_line(vec3(-scale,0,i), vec3(+scale,0,i), .color = V3_GRAY1, .thin = 1); // horiz
            dd_line(vec3(i,0,-scale), vec3(i,0,+scale), .color = V3_GRAY1, .thin = 1); // vert
        }

        dd_axis(.scale = scale);
    } else {
        dd_grid(.scale = 100);
        dd_grid(.scale = 10);
        dd_grid(.scale = 1);
        dd_grid(.scale = 0.1);
        dd_grid(.scale = 0.01);
    }
}
