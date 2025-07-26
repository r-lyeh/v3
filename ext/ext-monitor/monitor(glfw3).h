int monitors_(void) {
    int num_monitors = 0;
    GLFWmonitor **list = glfwGetMonitors(&num_monitors);
    return num_monitors;
}
double monitor_hz_(int monitor) {
    if( !monitor ) return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;

    int num_monitors = 0;
    GLFWmonitor **list = glfwGetMonitors(&num_monitors);
    assert((size_t)monitor <= num_monitors);
    return glfwGetVideoMode(list[monitor-1])->refreshRate;
}
double monitor_width_(int monitor) {
    if( !monitor ) return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;

    int num_monitors = 0;
    GLFWmonitor **list = glfwGetMonitors(&num_monitors);
    assert((size_t)monitor <= num_monitors);
    return glfwGetVideoMode(list[monitor-1])->width;
}
double monitor_height_(int monitor) {
    if( !monitor ) return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

    int num_monitors = 0;
    GLFWmonitor **list = glfwGetMonitors(&num_monitors);
    assert((size_t)monitor <= num_monitors);
    return glfwGetVideoMode(list[monitor-1])->height;
}
void monitor_workarea_(int monitor, int rect[4]) {
    memset(rect, 0, sizeof(int)*4); // @todo
}
