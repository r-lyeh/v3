// @todo: apply .hz
// @todo: .hz_bg: 5; reduce .hz while getwindowopacity<=0 || window == minimized

GLFWwindow* window;
char *extra;

void app_show(int show) {
    // technically, we should use glShow/HideWindow but app needs extra mouse click to regain focus that way.
    // tried to force glfwFocusWindow with/without glfwPollEvents after. no luck (glfw 3.4)
    //glfwSetWindowOpacity(window, show ? 1 : 0);
    (show ? glfwShowWindow : glfwHideWindow)(window);
    //if(show) glfwFocusWindow(window);
}

app_state_t app_state(void) {
    app_state_t st = {0};
    if( window ) {
        glfwGetFramebufferSize(window, &st.width, &st.height);
        st.aspect = st.w / (st.h + !st.h);
        st.handle = window;
        st.extra = extra;
        ifdef(V3_WINDOWS, st.handle_os = glfwGetWin32Window(window));
        glGetIntegerv(GL_SAMPLES, (GLint*)&st.aa);
    }
    return st;
}

bool (app_create)(app_create_flags opt) {
    if( window ) return false;

    extra = strdup(strvalid(opt.extra));

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    int xpos, ypos, ww, wh, mw = mode->width, mh = mode->height;
    glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &ww, &wh);

    if( opt.w * opt.h > 0 ) {
        opt.scale = (opt.w * opt.h) / (double)(mw * mh);
    } else {
        if( opt.scale <= 0 ) opt.scale = 0.75;
        if( opt.scale > 0 ) {
            while( opt.scale > 1.01 ) opt.scale /= 10;
            opt.w = mw * opt.scale;
            opt.h = mh * opt.scale;
        }
    }

//  if( opt.scale >= 1 ) xpos = ypos = 0, ww = mw, wh = mh;

#if 0
    // aspect ratio
    /**/ if( opt.aspect == 0 ) {}
    else if( opt.aspect == 1 ) opt.w = opt.h = min(opt.w, opt.h);
    else if( opt.aspect  > 1 ) opt.h /= opt.aspect;
    else if( opt.aspect  < 1 ) opt.w *= opt.aspect;
#endif

    // portrait/landscape
    // if( opt.vertical ) { float v = opt.w; opt.w = opt.h; opt.h = v; }
    // float v = min(opt.w, opt.h), opt.w = max(opt.w, opt.h), opt.h = v;

    printf("Monitor: %dHz (%dx%d > %dx%d @ %d,%d) %s\n", mode->refreshRate, mw,mh, ww,wh, xpos,ypos, glfwGetMonitorName(monitor));
    printf("Window: %d%% (%dx%d)\n", (int)(opt.s * 100), opt.w, opt.h);

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_VISIBLE, 0);
    glfwWindowHint(GLFW_WIN32_KEYBOARD_MENU, 0);
    glfwWindowHint(GLFW_RESIZABLE, opt.resizable);
    glfwWindowHint(GLFW_CENTER_CURSOR, 0);
    glfwWindowHint(GLFW_DECORATED, opt.decorated *= opt.scale < 1);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, opt.transparent);
    //glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, opt.transparent && opt.scale >= 1);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, 1);
    glfwWindowHint(GLFW_FOCUSED, 1);


    glfwWindowHint(GLFW_SAMPLES, opt.aa);
    glfwWindowHint(GLFW_SRGB_CAPABLE, 1);
#if V3_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, 1);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
#endif
    glfwWindowHint(GLFW_CONTEXT_DEBUG, opt.debug);
    glfwWindowHint(GLFW_CONTEXT_NO_ERROR, opt.silent);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_DOUBLEBUFFER, 1);

    // center windowed
    if( opt.scale < 1 ) {
    glfwWindowHint(GLFW_POSITION_X, xpos + (mw/*ww*/ - opt.w) / 2);
    glfwWindowHint(GLFW_POSITION_Y, ypos + (mh/*wh*/ - opt.h) / 2);
    }

    if( opt.scale > 1 ) {
    // soft fullscreen
    const GLFWvidmode* mode = glfwGetVideoMode(monitor); 
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    }

    // glfwShowWindow(window);

    // GLFWimage images[2];
    // images[0] = load_icon("my_icon.png");
    // images[1] = load_icon("my_icon_small.png");
    // glfwSetWindowIcon(window, 2, images);

    /* Create a windowed mode window and its OpenGL context */

    window = glfwCreateWindow(opt.w, opt.h, strvalid(opt.title), opt.scale > 1 ? monitor : NULL, NULL);
    if (!window) glfwWindowHint(GLFW_DEPTH_BITS, 24); // retry smaller depth (Intel)
    if (!window) window = glfwCreateWindow(opt.w, opt.h, strvalid(opt.title), opt.scale > 1 ? monitor : NULL, NULL);
    if (!window) die("Cannot create glfw window");

    if( opt.scale == 1 )
    glfwMaximizeWindow(window);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    event(on_graphics_context_created);

    glfwSwapInterval(0); // (0) max fps, (1) vsync, (-1) adaptive vsync

    if( opt.aspect )
    {
        // Multiply by a large number to approximate fraction
        int denominator = 1000; // Precision factor, adjust as needed
        int numerator = (int)(opt.aspect * denominator + 0.5); // Round to nearest int

        // Inline GCD calculation (Euclidean algorithm)
        int a = numerator, b = denominator;
        while (b) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        int divisor = a;

        // Simplify the fraction
        int numer = numerator / divisor;
        int denom = denominator / divisor;

        glfwSetWindowAspectRatio(window,numer,denom);
    }

    if( opt.visible ) glfwShowWindow(window);

            // Query actual status
            GLint samples, sample_buffers, depth_bits, stencil_bits, profile;
            GLboolean srgb_enabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
            glGetIntegerv(GL_SAMPLES, &samples);
            glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);
            glGetIntegerv(GL_DEPTH_BITS, &depth_bits);
            glGetIntegerv(GL_STENCIL_BITS, &stencil_bits);
            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile); profile = !!(profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT);

            int rx, ry, rw, rh;
            glfwGetWindowPos(window, &rx, &ry);
            glfwGetWindowSize(window, &rw, &rh);

            lputs(va("GPU device: %s", (const char*)glGetString(GL_RENDERER)));
            lputs(va("GPU driver: %s", (const char*)glGetString(GL_VERSION)));
            lputs(va("GPU compat: %d, depth: %d, stencil: %d, aa: %d, srgb: %d, rect: %dx%d@%d,%d", profile, depth_bits, stencil_bits, samples, srgb_enabled, rw,rh,rx,ry));

            ( opt.aa ? glEnable : glDisable )(GL_MULTISAMPLE);

    return true;
}
bool (app_swap)(app_swap_flags opt) {
    if( glfwWindowShouldClose(window) ) return false;

    static int seen = 0; if(!seen) { seen = 1;
        /* first frame */
    } else {
        event(on_frame_end);

        event(on_frame_swap);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll and process events */
        glfwPollEvents();
    }

    app_state_t st = app_state();
    glViewport(0,0,st.w,st.h);
    glScissor(0,0,st.w,st.h);

    // @todo: depthFunc(<,<=,>,>=)
    if( opt.color ) {
        unsigned a = (opt.color >> 24) & 255;
        unsigned b = (opt.color >> 16) & 255;
        unsigned g = (opt.color >>  8) & 255;
        unsigned r = (opt.color >>  0) & 255;
        glClearColor(r/255.,g/255.,b/255.,a/255.);
    }
    glClear(
        (opt.color < ~0u) * GL_COLOR_BUFFER_BIT +
        (opt.depth >= 0) * GL_DEPTH_BUFFER_BIT +
        (opt.stencil >= 0) * GL_STENCIL_BUFFER_BIT
    );

    event(on_frame_begin);

    return true;
}

AUTORUN {
    if (!glfwInit()) die("Cannot initialize glfw3 (" __FILE__ ")");
    atexit(glfwTerminate);
}
