size_t num_monitors;

int monitors_(void) {
    RGFW_monitor *list = RGFW_getMonitors(&num_monitors);
    return num_monitors;
}
double monitor_hz_(int monitor) {
    if( !monitor ) return RGFW_getPrimaryMonitor().mode.refreshRate;

    RGFW_monitor *list = RGFW_getMonitors(&num_monitors);
    assert((size_t)monitor <= num_monitors);
    return list[monitor-1].mode.refreshRate;
}
double monitor_width_(int monitor) {
    if( !monitor ) return RGFW_getPrimaryMonitor().mode.w;

    RGFW_monitor *list = RGFW_getMonitors(&num_monitors);
    assert((size_t)monitor <= num_monitors);
    return list[monitor-1].mode.w;
}
double monitor_height_(int monitor) {
    if( !monitor ) return RGFW_getPrimaryMonitor().mode.h;

    RGFW_monitor *list = RGFW_getMonitors(&num_monitors);
    assert((size_t)monitor <= num_monitors);
    return list[monitor-1].mode.h;
}
void monitor_workarea_(int monitor, int rect[4]) {
    memset(rect, 0, sizeof(int)*4); // @todo
}
