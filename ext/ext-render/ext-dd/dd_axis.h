void (dd_axis)(dd_flags opt) {
    float units = opt.scale;
    opt.color = V3_PINK;
    opt.dashed = 0; (dd_line)(vec3(0,0,0), vec3( units,0,0), opt);
    opt.dashed = 1; (dd_line)(vec3(0,0,0), vec3(-units,0,0), opt);
    opt.color = V3_GREEN;
    opt.dashed = 0; (dd_line)(vec3(0,0,0), vec3(0, units,0), opt);
    opt.dashed = 1; (dd_line)(vec3(0,0,0), vec3(0,-units,0), opt);
    opt.color = V3_CYAN;
    opt.dashed = 0; (dd_line)(vec3(0,0,0), vec3(0,0, units), opt);
    opt.dashed = 1; (dd_line)(vec3(0,0,0), vec3(0,0,-units), opt);
}
