// text input (unicode)

array_(unsigned) ime;

unsigned *keytext_(unsigned *count) {
    *count = array_count(ime);
    return ime;
}

// keys and keyboard state

int keyboard1[512 * 2], *keyboard0 = keyboard1 + 512, repeat1[ 512 ];
STATIC_ASSERT(GLFW_KEY_LAST < 512);

int *keyboard_(void) {
    return keyboard1;
}

int button_(int vk) {
    vk = ((unsigned)toupper(vk)) & 511;

    int flags =
        keyboard0[vk] > keyboard1[vk] ? key_up :
        keyboard0[vk] < keyboard1[vk] ? key_down :
        keyboard0[vk] > 0 && keyboard1[vk] > 0 ? key_held : key_idle;

    return flags;
}

int key2glfw_(int vk) {
    #define K(x) GLFW_KEY_##x

    const int non_printable_keyboard[] = {
/*00*/    K(ESCAPE),     K(F1),K(F2),K(F3),K(F4),    K(F5),K(F6),K(F7),K(F8),    K(F9),K(F10),K(F11),K(F12),
/*13*/                                                                                         K(BACKSPACE),
/*14*/    K(TAB),                                                                                  K(ENTER),
/*16*/    K(CAPS_LOCK),
/*17*/    K(LEFT_SHIFT),                                                                     K(RIGHT_SHIFT),           K(UP),
/*20*/    K(LEFT_CONTROL),K(LEFT_SUPER),K(LEFT_ALT),   K(RIGHT_ALT),K(RIGHT_SUPER),K(MENU),K(RIGHT_CONTROL),  K(LEFT),K(DOWN),K(RIGHT),
    };
    const int non_printable_keypad[] = {
/*00*/    K(PRINT_SCREEN), K(SCROLL_LOCK), K(PAUSE),
/*03*/    K(INSERT),       K(HOME),        K(PAGE_UP),    K(NUM_LOCK), K(KP_DIVIDE), K(KP_MULTIPLY), K(KP_SUBTRACT),
/*09*/    K(DELETE),       K(END),         K(PAGE_DOWN),  K(KP_7),     K(KP_8),      K(KP_9),        K(KP_ADD),
/*15*/                                                    K(KP_4),     K(KP_5),      K(KP_6),
/*17*/                                                    K(KP_1),     K(KP_2),      K(KP_3),        K(KP_ENTER),
/*20*/                                                    K(KP_0),                   K(KP_DECIMAL),
    };
    
    #undef K

    if( vk == KEY_LOCALE1 ) return GLFW_KEY_WORLD_1;
    if( vk == KEY_LOCALE2 ) return GLFW_KEY_WORLD_2;
    if( vk > 127 ) return non_printable_keypad[vk&127]   ? non_printable_keypad[vk&127] : vk&127;
    if( vk > 31  ) return vk;
    if( vk > 0   ) return non_printable_keyboard[vk    ] ? non_printable_keyboard[vk    ] : vk;
                   return non_printable_keyboard[0];
}

int key_(int vk) {
    return button_( key2glfw_(vk) );
}

#if 0
int key_repeat(unsigned char vk) {
    static int table[512] = {0};
    table[vk] *= !!key_pressed(vk);
    table[vk] += !!key_pressed(vk);
    return table[vk] == 1 || table[vk] > 38; // 38@60hz = 633ms
}
int key_longpress(unsigned char vk) {
    static int table[512] = {0};
    table[vk] *= !!key_pressed(vk);
    table[vk] += !!key_pressed(vk);
    return table[vk] == 48; // assumes 60hz==1.00s, 48==800s
}
#else
int keyrepeat_(int vk) {
    vk = ((unsigned)toupper(vk)) & 511;

    return button_(vk) == key_held ? repeat1[vk] : 0;
}
#endif

// callbacks

static void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if( key != GLFW_KEY_UNKNOWN )
    keyboard1[ key ] = action != GLFW_RELEASE,
    repeat1[ key ] = action == GLFW_REPEAT;
}
static void text_cb(GLFWwindow* window, unsigned int codepoint) {
    array_push(ime, codepoint);
}
static void keyboard_tick(void *unused) {
    memcpy(keyboard0, keyboard1, sizeof(int) * GLFW_KEY_LAST);

    array_clear(ime);

    ONCE {
        app_state_t app = app_state();
        void *window = app.handle;

        glfwSetKeyCallback(window, key_cb);
        glfwSetCharCallback(window, text_cb);
    }
}

AUTORUN {
    on_frame_end(keyboard_tick, NULL);
}
