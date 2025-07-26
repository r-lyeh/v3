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
        keyboard0[vk] > 0 && keyboard1[vk] > 0 ? key_repeat : key_idle;

    return flags;
}

int key_(int vk) {
    return button_(vk);
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

    return button_(vk) == key_repeat ? repeat1[vk] : 0;
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
static void keyboard_tick(void *nil) {
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
