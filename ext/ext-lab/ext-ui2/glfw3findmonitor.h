GLFWmonitor* glfwFindMonitor(GLFWwindow* handle) {
    // Validate window handle
    if (!handle) {
        // fprintf(stderr, "Error: Invalid GLFW window handle\n");
        return NULL;
    }

    // Check if window is iconified
    if (glfwGetWindowAttrib(handle, GLFW_ICONIFIED)) {
        // fprintf(stderr, "Warning: Window is iconified, returning primary monitor\n");
        return glfwGetPrimaryMonitor();
    }

    // Check if window is fullscreen
    GLFWmonitor* fullscreenMonitor = glfwGetWindowMonitor(handle);
    if (fullscreenMonitor) {
        return fullscreenMonitor; // Fullscreen windows are tied to a specific monitor
    }

    // Get window position and size
    int winX, winY, winWidth, winHeight;
    glfwGetWindowPos(handle, &winX, &winY);
    glfwGetWindowSize(handle, &winWidth, &winHeight);

    // Window rectangle
    int winLeft = winX;
    int winRight = winX + winWidth;
    int winTop = winY;
    int winBottom = winY + winHeight;

    // Get all monitors
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    if (!monitors || monitorCount == 0) {
        // fprintf(stderr, "Error: No monitors found\n");
        return NULL;
    }

    GLFWmonitor* primaryMonitor = NULL;
    float maxArea = -1.0f;

    // Iterate through each monitor
    for (int i = 0; i < monitorCount; ++i) {
        GLFWmonitor* monitor = monitors[i];

        // Get monitor position and size
        int monX, monY;
        glfwGetMonitorPos(monitor, &monX, &monY);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!mode) {
            continue; // Skip invalid monitors
        }
        int monWidth = mode->width;
        int monHeight = mode->height;

        // Monitor rectangle
        int monLeft = monX;
        int monRight = monX + monWidth;
        int monTop = monY;
        int monBottom = monY + monHeight;

        // Calculate intersection rectangle
        int interLeft = max(winLeft, monLeft);
        int interRight = min(winRight, monRight);
        int interTop = max(winTop, monTop);
        int interBottom = min(winBottom, monBottom);

        // Calculate intersection area
        int interWidth = interRight - interLeft;
        int interHeight = interBottom - interTop;
        float area = (interWidth > 0 && interHeight > 0) ? interWidth * interHeight : 0.0f;

        // Update primary monitor if this area is larger
        if (area > maxArea) {
            maxArea = area;
            primaryMonitor = monitor;
        }
    }

    // Fallback to primary monitor if no overlap is found
    if (!primaryMonitor) {
        // fprintf(stderr, "Warning: No monitor overlap found, returning primary monitor\n");
        return glfwGetPrimaryMonitor();
    }

    return primaryMonitor;
}
