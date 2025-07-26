bool glfwIsFullscreen(GLFWwindow* window) {
    bool fullscreen = (glfwGetWindowMonitor(window) != NULL);
    return fullscreen;
}
void glfwSetFullscreen(GLFWwindow* window, bool fs) {
    // last windowed mode (position and size)
    static int wx = 0, wy = 0, ww = 0, wh = 0;

    if( fs ^ glfwIsFullscreen(window) ) {
        if( fs ) {
            GLFWmonitor* monitor = glfwFindMonitor(window); // glfwGetPrimaryMonitor();
            if(!monitor) return;
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            if(!mode) return;

            // Save windowed state before going fullscreen
            glfwGetWindowPos(window, &wx, &wy);
            glfwGetWindowSize(window, &ww, &wh);

            // Switch to fullscreen
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            // Switch to windowed mode
            glfwSetWindowMonitor(window, NULL, wx, wy, ww, wh, 0);
        }
    }
}

enum { GLFW_MIN, GLFW_NORMAL, GLFW_MAX, GLFW_FULL };
int glfwGetWindowState(GLFWwindow *window) {
    int is_fullscreen = glfwIsFullscreen(window);
    int is_maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED) == GLFW_TRUE;
    int is_minimized = glfwGetWindowAttrib(window, GLFW_ICONIFIED) == GLFW_TRUE;
    int state = is_fullscreen ? 3 : is_maximized ? 2 : is_minimized ? 0 : 1;
    return state;
}
