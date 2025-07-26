#ifndef GAMEPAD_H
#define GAMEPAD_H

typedef int GamepadButton; enum { // typedef enum(GamepadButton) {
    GAMEPAD_UP,             // Digital D-pad up
    GAMEPAD_DOWN,           // Digital D-pad down
    GAMEPAD_LEFT,           // Digital D-pad left
    GAMEPAD_RIGHT,          // Digital D-pad right
    GAMEPAD_A,              // Face button A >< Cross
    GAMEPAD_B,              // Face button B () Circle
    GAMEPAD_X,              // Face button X [] Square
    GAMEPAD_Y,              // Face button Y /\ Triangle
    GAMEPAD_LB,             // Left bumper
    GAMEPAD_RB,             // Right bumper
    GAMEPAD_LS,             // Left stick click
    GAMEPAD_RS,             // Right stick click
    GAMEPAD_LX,             // Left analog stick, horizontal axis
    GAMEPAD_LY,             // Left analog stick, vertical axis
    GAMEPAD_RX,             // Right analog stick, horizontal axis
    GAMEPAD_RY,             // Right analog stick, vertical axis
    GAMEPAD_LT,             // Left analog trigger
    GAMEPAD_RT,             // Right analog trigger
    GAMEPAD_BACK,           // Menu / Options button
    GAMEPAD_GUIDE,          // Guide / Home / PS button
    GAMEPAD_START,          // Start button (if distinct from Menu)
    GAMEPAD_SHARE,          // Share / Capture button
    GAMEPAD_COUNT,          // Sentinel for number of buttons

    // aliases: north, south, east, west
    GAMEPAD_N = GAMEPAD_Y,  // Face button Y /\ Triangle
    GAMEPAD_S = GAMEPAD_A,  // Face button A >< Cross
    GAMEPAD_E = GAMEPAD_B,  // Face button B () Circle
    GAMEPAD_W = GAMEPAD_X,  // Face button X [] Square
};

extern float (*gamepad_live)(int id); // 0 if disconnected, >0 if connected (battery level (0..100])
extern float (*gamepad_down)(int id, GamepadButton key);
extern float (*gamepad_held)(int id, GamepadButton key);
extern float (*gamepad_idle)(int id, GamepadButton key);
extern float (*gamepad_read)(int id, GamepadButton key);
extern void**(*gamepad_data)(int id);
extern int   (*gamepad_count)(void);

#define GAMEPAD_FOREACH(X) \
    X(GAMEPAD_UP)             /* Digital D-pad up */ \
    X(GAMEPAD_DOWN)           /* Digital D-pad down */ \
    X(GAMEPAD_LEFT)           /* Digital D-pad left */ \
    X(GAMEPAD_RIGHT)          /* Digital D-pad right */ \
    X(GAMEPAD_A)              /* Face button A >< Cross */ \
    X(GAMEPAD_B)              /* Face button B () Circle */ \
    X(GAMEPAD_X)              /* Face button X [] Square */ \
    X(GAMEPAD_Y)              /* Face button Y /\ Triangle */ \
    X(GAMEPAD_LB)             /* Left bumper */ \
    X(GAMEPAD_RB)             /* Right bumper */ \
    X(GAMEPAD_LS)             /* Left stick click */ \
    X(GAMEPAD_RS)             /* Right stick click */ \
    X(GAMEPAD_LX)             /* Left analog stick, horizontal axis */ \
    X(GAMEPAD_LY)             /* Left analog stick, vertical axis */ \
    X(GAMEPAD_RX)             /* Right analog stick, horizontal axis */ \
    X(GAMEPAD_RY)             /* Right analog stick, vertical axis */ \
    X(GAMEPAD_LT)             /* Left trigger (digital, if treated as button) */ \
    X(GAMEPAD_RT)             /* Right trigger (digital, if treated as button) */ \
    X(GAMEPAD_BACK)           /* Menu / Options button */ \
    X(GAMEPAD_GUIDE)          /* Guide / Home / PS button */ \
    X(GAMEPAD_START)          /* Start button (if distinct from Menu) */ \
    X(GAMEPAD_SHARE)          /* Share / Capture button */ \
//  X(GAMEPAD_COUNT)          /* Sentinel for number of buttons */

#elif CODE
#pragma once
#include v3_gamepad_backend

float (*gamepad_live)(int) = gamepad_live_;
float (*gamepad_down)(int, GamepadButton) = gamepad_down_;
float (*gamepad_held)(int, GamepadButton) = gamepad_held_;
float (*gamepad_idle)(int, GamepadButton) = gamepad_idle_;
float (*gamepad_read)(int, GamepadButton) = gamepad_read_;
void**(*gamepad_data)(int id) = gamepad_data_;
int   (*gamepad_count)(void) = gamepad_count_;

#endif
