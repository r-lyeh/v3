// mouse records (either coordinates or buttons)

int mouse1[16 * 2], *mouse0 = mouse1 + 16;

int mbutton_(int vk) {
    vk = ((unsigned)toupper(vk)) & 15;

    int flags =
        mouse0[vk] > mouse1[vk] ? mouse_up :
        mouse0[vk] < mouse1[vk] ? mouse_down :
        mouse0[vk] > 0 && mouse1[vk] > 0 ? mouse_held : mouse_idle;

    return flags;
}

// mouse state

enum {
    M0 = '\1', M1, M2, MX, MY, MZ, MU, MV, MW
};

int mouse_(int vk) {
    const int map[] = {
        ['X'] = MX, ['x'] = MX,
        ['Y'] = MY, ['y'] = MY,
        ['U'] = MU, ['u'] = MU,
        ['V'] = MV, ['v'] = MV,
        ['W'] = MW, ['w'] = MW,
        ['L'] = M0, ['l'] = M0, ['0'] = M0, [0] = M0,
        ['M'] = M1, ['m'] = M1, ['1'] = M1, [1] = M1,
        ['R'] = M2, ['r'] = M2, ['2'] = M2, [2] = M2,
    };
    vk = vk <= 'y' ? map[vk] : 0;
    if( vk >= MX ) return mouse1[vk];
    if( vk >= M0 ) return mbutton_(vk);
    return 0;
}
int mouse_cursor_(int mode) {
    static GLFWcursor *cursors[7] = {0};
    ONCE {
    cursors[1] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    cursors[2] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    cursors[3] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR) ;
    cursors[4] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    cursors[5] = glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR); // @todo? resizeEW,resizeNS,resizeNWSE,resizeNESW,
    cursors[6] = glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR);
    };
    static int last = 0;
    if( mode != last )
    if( mode >= 0 && mode < COUNTOF(cursors) ) {
        void *window = app_state().handle;
        if( mode > 0 ) glfwSetCursor(window, cursors[mode]);

        glfwSetInputMode(window, GLFW_CURSOR, mode > 0 ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

        if( glfwRawMouseMotionSupported() )
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, mode <= 0);

        return last = mode;
    }
    return last;
}

// callbacks

static void cursor_cb(GLFWwindow* window, double xpos, double ypos) {
    mouse1[MX] = xpos, mouse1[MY] = ypos;
}
static void scroll_cb(GLFWwindow* window, double xoffset, double yoffset) {
    mouse1[MZ] -= yoffset * 100;
}
static void mouse_cb(GLFWwindow* window, int button, int action, int mods) {
    action = action == GLFW_PRESS;
    /**/ if( button == GLFW_MOUSE_BUTTON_LEFT )   mouse1[M0] = action;
    else if( button == GLFW_MOUSE_BUTTON_MIDDLE ) mouse1[M1] = action;
    else if( button == GLFW_MOUSE_BUTTON_RIGHT )  mouse1[M2] = action;
}
static void mouse_tick(void *unused) {
    mouse1[MU] = mouse1[MX] - mouse0[MX];
    mouse1[MV] = mouse1[MY] - mouse0[MY];
    mouse1[MW] = mouse1[MZ] - mouse0[MZ];

    memcpy(mouse0, mouse1, sizeof(int) * 16);

    ONCE {
        app_state_t app = app_state();
        void *window = app.handle;

        glfwSetScrollCallback(window, scroll_cb);
        glfwSetCursorPosCallback(window, cursor_cb);
        glfwSetMouseButtonCallback(window, mouse_cb);
    }
}

AUTORUN {
    on_frame_end(mouse_tick, NULL);
}
