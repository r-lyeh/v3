#ifndef KEYBOARD_H
#define KEYBOARD_H

enum {
    key_idle, key_held, key_down, key_up,
};

extern int    (*key)(int vk);
extern int    (*keyrepeat)(int vk);
extern int*   (*keyboard)(void);

extern unsigned * (*keytext)(unsigned *count);

#define key_up(vk)   (key(vk) == key_up)
#define key_down(vk) (key(vk) == key_down)
#define key_held(vk) (key(vk) == key_held)
#define key_idle(vk) (key(vk) == key_idle)

#define FOREACH_KEY(k) \
    k(ESC) k(F1)k(F2)k(F3)k(F4)k(F5)k(F6)k(F7)k(F8)k(F9)k(F10)k(F11)k(F12) \
                                                                     k(BS) \
    k(TAB)                                                        k(ENTER) \
    k(CAPS)                                                                \
    k(LSHIFT)                                                     k(RSHIFT)           k(UP) \
    k(LCTRL)k(LSUPER)k(LALT)                k(RALT)k(RSUPER)k(MENU)k(RCTRL)  k(LEFT)k(DOWN)k(RIGHT)

#define FOREACH_KEYPAD(k) \
    k(PRINT) k(SCROLL)  k(PAUSE) \
    k(INS)   k(HOME)    k(PGUP)    k(KPNUM) k(KPDIV) k(KPMUL) k(KPSUB) \
    k(DEL)   k(END)     k(PGDOWN)  k(KP7)   k(KP8)   k(KP9)   k(KPADD) \
                                   k(KP4)   k(KP5)   k(KP6)            \
                                   k(KP1)   k(KP2)   k(KP3)   k(KPENT) \
                                   k(KP0)            k(KPDEC)

#define FOREACH_KEYLOCAL(k) \
    k(SPACE) k(LOCALE1) k(LOCALE2) k(DUMMY35) k(DUMMY36) k(DUMMY37) k(DUMMY38) \
    k(APOS) k(DUMMY40) k(DUMMY41) k(DUMMY42) k(DUMMY43) k(COMMA) k(MINUS) k(DOT) k(SLASH) \
    k(0) k(1) k(2) k(3) k(4) k(5) k(6) k(7) k(8) k(9) \
    k(DUMMY58) k(SEMICOLON) k(DUMMY60) k(EQUAL) k(DUMMY62) k(DUMMY63) k(DUMMY64) \
    k(A) k(B) k(C) k(D) k(E) k(F) k(G) k(H) k(I) k(J) k(K) k(L) k(M) k(N) k(O) \
    k(P) k(Q) k(R) k(S) k(T) k(U) k(V) k(W) k(X) k(Y) k(Z) k(LBRACKET) k(BACKSLASH) k(RBRACKET) \
    k(DUMMY94) k(DUMMY95) k(TILDE)

enum {
    #define K(x) KEY_##x,
    FOREACH_KEY(K)
    KEY_DUMMY31 = 31,
    FOREACH_KEYLOCAL(K)
    KEY_DUMMY127 = 127,
    FOREACH_KEYPAD(K)
    #undef K
};

#elif CODE
#pragma once
#include v3_keyboard_backend

int (*key)(int vk) = key_;
int (*keyrepeat)(int vk) = keyrepeat_;
int*(*keyboard)(void) = keyboard_;

unsigned *(*keytext)(unsigned *count) = keytext_;

#endif
