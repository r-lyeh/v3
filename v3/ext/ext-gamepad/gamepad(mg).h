#ifdef _WIN32
#include <winsock2.h>
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#pragma comment(lib, "user32") // __imp_GetRawInputDeviceList
#else
#include <unistd.h>
#if V3_LINUX
#include <linux/input.h>
#ifndef ABS_PROFILE
#define ABS_PROFILE 0x21 // @todo temp hack: remove when minigamepad.h gets fixed
#endif
#elif V3_MACOS // @todo temp hack: remove when minigamepad.h gets fixed
#define kHIDUsage_Button_5  0x5
#define kHIDUsage_Button_6  0x6
#define kHIDUsage_Button_7  0x7
#define kHIDUsage_Button_8  0x8
#define kHIDUsage_Button_9  0x9
#define kHIDUsage_Button_10 0xA
#endif
#endif

#define MG_API
#define MG_IMPLEMENTATION
#include "3rd/minigamepad.h"

struct gamepad_t {
    float power;
    float buttons0[GAMEPAD_COUNT]; // prev state
    float buttons1[GAMEPAD_COUNT]; // curr state
    char  name[64];
} pad[4];

void gamepad_update_(void *arg) {
    static mg_gamepads gamepads = {0};
    static int map_buttons[] = {
        [MG_BUTTON_DPAD_UP] = GAMEPAD_UP,
        [MG_BUTTON_DPAD_DOWN] = GAMEPAD_DOWN,
        [MG_BUTTON_DPAD_LEFT] = GAMEPAD_LEFT,
        [MG_BUTTON_DPAD_RIGHT] = GAMEPAD_RIGHT,
        [MG_BUTTON_SOUTH] = GAMEPAD_A,
        [MG_BUTTON_EAST] = GAMEPAD_B,
        [MG_BUTTON_WEST] = GAMEPAD_X,
        [MG_BUTTON_NORTH] = GAMEPAD_Y,
        [MG_BUTTON_LEFT_SHOULDER] = GAMEPAD_LB,
        [MG_BUTTON_RIGHT_SHOULDER] = GAMEPAD_RB,
        [MG_BUTTON_LEFT_TRIGGER] = GAMEPAD_LT,
        [MG_BUTTON_RIGHT_TRIGGER] = GAMEPAD_RT,
        [MG_BUTTON_LEFT_STICK] = GAMEPAD_LS,
        [MG_BUTTON_RIGHT_STICK] = GAMEPAD_RS,
        [MG_BUTTON_BACK] = GAMEPAD_BACK,
        [MG_BUTTON_GUIDE] = GAMEPAD_GUIDE,
        [MG_BUTTON_START] = GAMEPAD_START,
        [MG_BUTTON_MISC1] = GAMEPAD_SHARE,
    };
    static int map_axes[] = {
        [MG_AXIS_LEFT_X] = GAMEPAD_LX,
        [MG_AXIS_LEFT_Y] = GAMEPAD_LY,
        [MG_AXIS_RIGHT_X] = GAMEPAD_RX,
        [MG_AXIS_RIGHT_Y] = GAMEPAD_RY,
        [MG_AXIS_LEFT_TRIGGER] = GAMEPAD_LT,
        [MG_AXIS_RIGHT_TRIGGER] = GAMEPAD_RT,
    };

    ONCE mg_gamepads_init(&gamepads); // @leak

    while(mg_gamepads_update(&gamepads, NULL));

    mg_gamepad* gamepad = gamepads.list.head;

    int id = 0;
    for( ; gamepad ; gamepad = gamepad->next, ++id ) {
        if( id >= 4 ) continue;

        struct gamepad_t *g = &pad[id];
        memcpy(g->buttons0, g->buttons1, sizeof(g->buttons0));
        memset(g->buttons1,           0, sizeof(g->buttons1));

        snprintf(g->name, 64, "%s", gamepad->name);

        g->power = 100 * !!gamepad->connected;

        for( mg_button btn = 0; btn <= MG_BUTTON_MISC1 /*MG_BUTTON_COUNT*/; btn++ ) {
            if (!gamepad->buttons[btn].supported) continue;

            int v = gamepad->buttons[btn].current;
            g->buttons1[ map_buttons[btn] ] = v;
            // printf("%25s = %d ; %d>%d\n", mg_button_get_name(btn), v, btn, map_buttons[btn]);
        }

        for( mg_axis axis = 0; axis <= MG_AXIS_RIGHT_TRIGGER /*MG_AXIS_COUNT*/; axis++ ) {
            if (!gamepad->axes[axis].supported) continue;

            float v = gamepad->axes[axis].value;
            g->buttons1[ map_axes[axis] ] = v;
            // printf("%25s = %f ; %d>%d\n", mg_button_get_name(axis), v, btn, map_buttons[btn]);
        }

        // flip Y sign
        g->buttons1[GAMEPAD_LY] *= -1;
        g->buttons1[GAMEPAD_RY] *= -1;
        // remap [-1..1] to [0..1]
        g->buttons1[GAMEPAD_LT] = (g->buttons1[GAMEPAD_LT] + 1) * 0.5;
        g->buttons1[GAMEPAD_RT] = (g->buttons1[GAMEPAD_RT] + 1) * 0.5;
    }
}

int gamepad_count_(void) {
    return !!pad[0].power + !!pad[1].power + !!pad[2].power + !!pad[3].power;
}
float gamepad_live_(int id) {
    if( id < 0 || id >= 4 ) return 0;
    return pad[id].power;
}
float gamepad_down_(int id, GamepadButton key) {
    if( id < 0 || id >= 4 ) return 0;
    float lo0 = fabs(pad[id].buttons0[key]) < 0.5;
    float lo1 = fabs(pad[id].buttons1[key]) < 0.5;
    return lo0 && !lo1;
}
float gamepad_held_(int id, GamepadButton key) {
    if( id < 0 || id >= 4 ) return 0;
    float lo0 = fabs(pad[id].buttons0[key]) < 0.5;
    float lo1 = fabs(pad[id].buttons1[key]) < 0.5;
    return lo0 && lo1;
}
float gamepad_idle_(int id, GamepadButton key) {
    if( id < 0 || id >= 4 ) return 0;
    float lo0 = fabs(pad[id].buttons0[key]) < 0.5;
    float lo1 = fabs(pad[id].buttons1[key]) < 0.5;
    return !lo0 && !lo1;
}
float gamepad_read_(int id, GamepadButton key) {
    if( id < 0 || id >= 4 ) return 0;
    return pad[id].buttons1[key];
}

AUTORUN {
    on_frame_begin(gamepad_update_, NULL);
}
