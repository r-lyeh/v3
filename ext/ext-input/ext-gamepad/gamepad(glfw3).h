// @todo: battery level. leds. padtrack. rumble

struct GamepadInfo {
    char guid[33];  // GUID is 32 hex chars + null terminator
    int id;         // Current joystick ID (-1 if disconnected)
    int player;     // Your custom assignment (e.g., 0 for Player 1)
    void *userdata; // User-defined player datas
    int connected;
    int power;
    float state0[GAMEPAD_COUNT], state1[GAMEPAD_COUNT];
} pad[4] = {0};  // Initialize to zeros

enum { _4 = COUNTOF(pad) };

// Callback for connect/disconnect
void gamepad_cb(int jid, int event) {
    if( event == GLFW_CONNECTED ) {
        if( !glfwJoystickIsGamepad(jid) ) return;

        const char* guid = glfwGetJoystickGUID(jid);
        if( guid ) {
            printf("Gamepad[%d] joined with GUID: %s\n", jid, guid);
            
            // Check if we know this GUID (reconnect)
            int found = -1;
            for( int i = 0; i < _4; ++i ) {
                if( strcmp(pad[i].guid, guid) == 0 ) {
                    found = i;
                    break;
                }
            }
            
            if( found == -1 ) {
                // New device: Find empty slot
                for( int i = 0; i < _4; ++i ) {
                    if( !pad[i].connected ) {
                        found = i;
                        strncpy(pad[i].guid, guid, 32);
                        pad[i].player = i;  // Assign player based on slot
                        break;
                    }
                }
            }
            
            if( found != -1 ) {
                pad[found].id = jid;
                pad[found].connected = 1;
                pad[found].power = 100;
                printf("Gamepad[%d] name: %s\n", found, glfwGetGamepadName(jid));
            } else {
                printf("No slots available for new gamepad.\n");
            }
        }
    } else if( event == GLFW_DISCONNECTED ) {
        // Mark as disconnected but keep GUID for reconnect
        for( int i = 0; i < _4; ++i ) {
            if( pad[i].id == jid ) {
                printf("Gamepad[%d] parted\n", jid);
                pad[i].id = -1;
                pad[i].connected = 0;
                pad[i].power = 0;
                break;
            }
        }
    }
}
void gamepad_tick(void *nope) {
    ONCE {
        // update gamepad mappings from external file if possible
        const char* mappings = asset("gamecontrollerdb.txt").str;
        if( mappings ) glfwUpdateGamepadMappings(mappings);

        // Initial scan for already connected pad
        for( int jid = 0; jid < GLFW_JOYSTICK_LAST+1; ++jid ) {
            if( glfwJoystickPresent(jid) ) {
                gamepad_cb(jid, GLFW_CONNECTED);
            }
        }
        // Set callback
        glfwSetJoystickCallback(gamepad_cb);
    }

    // Process inputs for tracked pad
    for( int i = 0; i < _4; ++i ) {
        memcpy(&pad[i].state0, &pad[i].state1, sizeof(pad[i].state1));

        if( pad[i].connected && pad[i].id != -1 ) {
            GLFWgamepadstate state;
            if( glfwGetGamepadState(pad[i].id, &state)) {

                float *b = pad[i].state1;

                // digital buttons 0/1 // @todo: SHARE button
                b[GAMEPAD_UP] = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
                b[GAMEPAD_DOWN] = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
                b[GAMEPAD_LEFT] = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
                b[GAMEPAD_RIGHT] = state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];
                b[GAMEPAD_A] = state.buttons[GLFW_GAMEPAD_BUTTON_A];
                b[GAMEPAD_B] = state.buttons[GLFW_GAMEPAD_BUTTON_B];
                b[GAMEPAD_X] = state.buttons[GLFW_GAMEPAD_BUTTON_X];
                b[GAMEPAD_Y] = state.buttons[GLFW_GAMEPAD_BUTTON_Y];
                b[GAMEPAD_LB] = state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER];
                b[GAMEPAD_RB] = state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];
                b[GAMEPAD_LS] = state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB];
                b[GAMEPAD_RS] = state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB];
                b[GAMEPAD_BACK] = state.buttons[GLFW_GAMEPAD_BUTTON_BACK];
                b[GAMEPAD_GUIDE] = state.buttons[GLFW_GAMEPAD_BUTTON_GUIDE];
                b[GAMEPAD_START] = state.buttons[GLFW_GAMEPAD_BUTTON_START];
                
                // analog axes [-1.0 .. 1.0]
                b[GAMEPAD_LX] = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                b[GAMEPAD_LY] = -state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
                b[GAMEPAD_RX] = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                b[GAMEPAD_RY] = -state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
                b[GAMEPAD_LT] =  state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
                b[GAMEPAD_RT] =  state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

                continue;
            }
        }

        memset(&pad[i].state1, 0, sizeof(pad[i].state1));
    }
}
AUTORUN {
    on_frame_begin(gamepad_tick, NULL);
}


int gamepad_count_(void) {
    return !!pad[0].power + !!pad[1].power + !!pad[2].power + !!pad[3].power;
}
float gamepad_live_(int id) { // 0 if disconnected, >0 if connected (battery level (0..100])
    if( id < 0 || id >= _4 ) return 0;
    return pad[id].power;
}
float gamepad_down_(int id, GamepadButton key) {
    if( id < 0 || id >= _4 ) return 0;
    float lo0 = fabs(pad[id].state0[key]) < 0.5;
    float lo1 = fabs(pad[id].state1[key]) < 0.5;
    return lo0 && !lo1;
}
float gamepad_held_(int id, GamepadButton key) {
    if( id < 0 || id >= _4 ) return 0;
    float lo0 = fabs(pad[id].state0[key]) < 0.5;
    float lo1 = fabs(pad[id].state1[key]) < 0.5;
    return lo0 && lo1;
}
float gamepad_idle_(int id, GamepadButton key) {
    if( id < 0 || id >= _4 ) return 0;
    float lo0 = fabs(pad[id].state0[key]) < 0.5;
    float lo1 = fabs(pad[id].state1[key]) < 0.5;
    return !lo0 && !lo1;
}
float gamepad_read_(int id, GamepadButton key) {
    if( id < 0 || id >= _4 ) return 0;
    return pad[id].state1[key];
}

void** gamepad_data_(int id) {
    if( id < 0 || id >= _4 ) return 0;
    return &pad[id].userdata;
}

#if 0

typedef DWORD (WINAPI * PFN_XInputSetState)(DWORD,XINPUT_VIBRATION*);

#define XInputSetState _glfw.win32.xinput.SetState

_glfw.win32.xinput.SetState = (PFN_XInputSetState)
    GetProcAddress(_glfw.win32.xinput.instance, "XInputSetState");

int _glfwPlatformSetJoystickRumble(_GLFWjoystick* js, float slowMotorIntensity01, float fastMotorIntensity01)
{
    XINPUT_VIBRATION effect;

    if (js->win32.device)
        return GLFW_FALSE;

    if( slowMotorIntensity01 < 0 ) slowMotorIntensity01 = 0;
    if( slowMotorIntensity01 > 1 ) slowMotorIntensity01 = 1;
    if( fastMotorIntensity01 < 0 ) fastMotorIntensity01 = 0;
    if( fastMotorIntensity01 > 1 ) fastMotorIntensity01 = 1;

    ZeroMemory(&effect, sizeof(XINPUT_VIBRATION));

    effect.wLeftMotorSpeed  = (WORD)(65535.0f * slowMotorIntensity01);
    effect.wRightMotorSpeed = (WORD)(65535.0f * fastMotorIntensity01);

    return (int) (XInputSetState(js->win32.index, &effect) == ERROR_SUCCESS);
}

#endif

#if 0

linjs.fd = open(path, O_RDWR | O_NONBLOCK);

static void initJoystickForceFeedback(_GLFWjoystickLinux *linjs)
{
    linjs->rumble = NULL;
    struct ff_effect* effect = NULL;

    char ffBits[(FF_CNT + 7) / 8] = {0};
    if (ioctl(linjs->fd, EVIOCGBIT(EV_FF, sizeof(ffBits)), ffBits) < 0)
    {
        return;
    }

    if (isBitSet(FF_RUMBLE, ffBits))
    {
        effect = malloc(sizeof(struct ff_effect));
        *effect = (struct ff_effect)
        {
            .type      = FF_RUMBLE,
            .id        = -1,
            .direction = 0,
            .trigger   = {
                .button   = 0,
                .interval = 0
            },
            .replay = {
                .length = 2000,  // xinput rumble lasts ~2 seconds
                .delay  = 0
            },
            .u.rumble = {
                .strong_magnitude = 0,
                .weak_magnitude   = 0
            }
        };

        if (ioctl(linjs->fd, EVIOCSFF, effect) < 0)
        {
            free(effect);
        } else {
            linjs->rumble = effect;
        }
    }
}

int _glfwPlatformSetJoystickRumble(_GLFWjoystick* js, float slowMotorIntensity, float fastMotorIntensity)
{
    _GLFWjoystickLinux *linjs = &js->linjs;

    if (!js->linjs.rumble)
        return GLFW_FALSE;

    js->linjs.rumble->u.rumble = (struct ff_rumble_effect)
    {
        .strong_magnitude = 65535 * slowMotorIntensity,
        .weak_magnitude   = 65535 * fastMotorIntensity
    };

    struct input_event play =
    {
        .type  = EV_FF,
        .code  = linjs->rumble->id,
        .value = 1
    };

    if (ioctl(linjs->fd, EVIOCSFF, linjs->rumble) < 0 ||
        write(linjs->fd, (const void*) &play, sizeof(play)) < 0)
    {
        return GLFW_FALSE;
    }

    return GLFW_TRUE;
}

#endif
