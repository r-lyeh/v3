int glfwMoveUndecoratedWindow(GLFWwindow* window) {
    static double s_xpos = 0, s_ypos = 0;
    static int w_xsiz = 0, w_ysiz = 0;
    static int dragState = 0;
    if (glfwGetMouseButton(window, 0) == GLFW_PRESS && dragState == 0) {
        glfwGetCursorPos(window, &s_xpos, &s_ypos);
        glfwGetWindowSize(window, &w_xsiz, &w_ysiz);
        dragState = 1;
    }
    if (glfwGetMouseButton(window, 0) == GLFW_PRESS && dragState == 1) {
        double c_xpos, c_ypos;
        int w_xpos, w_ypos;
        glfwGetCursorPos(window, &c_xpos, &c_ypos);
        glfwGetWindowPos(window, &w_xpos, &w_ypos);
        if (
            s_xpos >= 0 && s_xpos <= ((double)w_xsiz - 170) &&
            s_ypos >= 0 && s_ypos <= 25) {
            glfwSetWindowPos(window, w_xpos + (c_xpos - s_xpos), w_ypos + (c_ypos - s_ypos));
        }
        if (
            s_xpos >= ((double)w_xsiz - 15) && s_xpos <= ((double)w_xsiz) &&
            s_ypos >= ((double)w_ysiz - 15) && s_ypos <= ((double)w_ysiz)) {
            glfwSetWindowSize(window, w_xsiz + (c_xpos - s_xpos), w_ysiz + (c_ypos - s_ypos));
        }
    }
    if (glfwGetMouseButton(window, 0) == GLFW_RELEASE && dragState == 1) {
        dragState = 0;
    }
    return dragState;
}

int glfwResizeUndecoratedWindow(GLFWwindow *window, unsigned icons) {

    typedef enum {
        NONE,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    } ResizeSide;
        
    static ResizeSide current_side = NONE;
    static bool is_resizing = false;
    static double start_mouse_x, start_mouse_y;
    static int start_win_x, start_win_y, start_width, start_height;
    static const int min_width = 200;
    static const int min_height = 150;
    static const int border_width = 5;
    static bool mouse_was_down = false;
    static GLFWcursor* arrowCursor;
    static GLFWcursor* ewResizeCursor;
    static GLFWcursor* nsResizeCursor;
    static GLFWcursor* nwseResizeCursor;
    static GLFWcursor* neswResizeCursor;

    if(!arrowCursor) arrowCursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR); // @leak
    if(!ewResizeCursor) ewResizeCursor = glfwCreateStandardCursor(GLFW_RESIZE_EW_CURSOR); // @leak
    if(!nsResizeCursor) nsResizeCursor = glfwCreateStandardCursor(GLFW_RESIZE_NS_CURSOR); // @leak
    if(!nwseResizeCursor) nwseResizeCursor = glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR); // @leak
    if(!neswResizeCursor) neswResizeCursor = glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR); // @leak

    // Get current cursor position (relative to window)
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Get current window size and position
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    int win_x, win_y;
    glfwGetWindowPos(window, &win_x, &win_y);

    // Compute absolute mouse position
    double mouse_screen_x = win_x + xpos;
    double mouse_screen_y = win_y + ypos;

    // Get mouse button state
    bool mouse_down = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

        // Set appropriate cursor
        if (current_side == TOP_LEFT || current_side == BOTTOM_RIGHT) {
            if(icons&16) glfwSetCursor(window, nwseResizeCursor);
        } else if (current_side == TOP_RIGHT || current_side == BOTTOM_LEFT) {
            if(icons&8) glfwSetCursor(window, neswResizeCursor);
        } else if (current_side == LEFT || current_side == RIGHT) {
            if(icons&4) glfwSetCursor(window, ewResizeCursor);
        } else if (current_side == TOP || current_side == BOTTOM) {
            if(icons&2) glfwSetCursor(window, nsResizeCursor);
        } else {
            if(icons&1) glfwSetCursor(window, arrowCursor);
        }

    if (!is_resizing) {
        // Determine if mouse is hovering over resize borders
        bool left = (xpos < border_width);
        bool right = (xpos > width - border_width);
        bool top = (ypos < border_width);
        bool bottom = (ypos > height - border_width);

        if (left && top) current_side = TOP_LEFT;
        else if (left && bottom) current_side = BOTTOM_LEFT;
        else if (right && top) current_side = TOP_RIGHT;
        else if (right && bottom) current_side = BOTTOM_RIGHT;
        else if (left) current_side = LEFT;
        else if (right) current_side = RIGHT;
        else if (top) current_side = TOP;
        else if (bottom) current_side = BOTTOM;
        else current_side = NONE;

        // Check for resize start
        if (mouse_down && !mouse_was_down && current_side != NONE) {
            is_resizing = true;
            start_mouse_x = mouse_screen_x;
            start_mouse_y = mouse_screen_y;
            start_win_x = win_x;
            start_win_y = win_y;
            start_width = width;
            start_height = height;
        }
    } else {
        // Handle ongoing resize
        if (!mouse_down) {
            is_resizing = false;
            current_side = NONE;
        } else {
            double dx = mouse_screen_x - start_mouse_x;
            double dy = mouse_screen_y - start_mouse_y;

            int new_x = start_win_x;
            int new_y = start_win_y;
            int new_width = start_width;
            int new_height = start_height;

            // Adjust size and position based on resize side
            if (current_side == LEFT || current_side == TOP_LEFT || current_side == BOTTOM_LEFT) {
                new_width = (int)(start_width - dx);
                new_x = (int)(start_win_x + dx);
            }
            if (current_side == RIGHT || current_side == TOP_RIGHT || current_side == BOTTOM_RIGHT) {
                new_width = (int)(start_width + dx);
            }
            if (current_side == TOP || current_side == TOP_LEFT || current_side == TOP_RIGHT) {
                new_height = (int)(start_height - dy);
                new_y = (int)(start_win_y + dy);
            }
            if (current_side == BOTTOM || current_side == BOTTOM_LEFT || current_side == BOTTOM_RIGHT) {
                new_height = (int)(start_height + dy);
            }

            // Clamp to minimum size
            if (new_width < min_width) new_width = min_width;
            if (new_height < min_height) new_height = min_height;

            // Apply new size and position
            glfwSetWindowSize(window, new_width, new_height);
            glfwSetWindowPos(window, new_x, new_y);
        }
    }

    mouse_was_down = mouse_down;
    return is_resizing;
}
