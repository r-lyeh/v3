#if CODE
#pragma once

int mouse_x_[2], mouse_y_[2], mouse_wheel, mouse_button[3];
int up[RGFW_keyLast],   up_count = 0;   // keyboard ime
int down[RGFW_keyLast], down_count = 0; // keyboard ime
int text[RGFW_keyLast], text_count = 0; // keyboard ime
int keyboard[RGFW_keyLast];

int key_(int vk) {
    static const int keycodes[256] = { ['\x1b'] = RGFW_escape };
    vk = tolower(vk);
    return RGFW_isKeyDown(vk); // keycodes[vk]); // RGFW_isKeyPressed
}

int mouse_(int vk) {
    static const int* addr[] = {
        ['x'] = &mouse_x_[0], ['y'] = &mouse_y_[0], ['w'] = &mouse_wheel,
        ['l'] = &mouse_button[0], ['r'] = &mouse_button[1], ['m'] = &mouse_button[2],
        ['0'] = &mouse_button[0], ['1'] = &mouse_button[1], ['2'] = &mouse_button[2],
        [ 0 ] = &mouse_button[0], [ 1 ] = &mouse_button[1], [ 2 ] = &mouse_button[2],
    };
    if( vk == 'u' ) return mouse_x_[0] - mouse_x_[1];
    if( vk == 'v' ) return mouse_y_[0] - mouse_y_[1];
    return addr[vk][0];
}

int mouse_cursor_(int mode) {
    static const int modes[] = {
        0,
        RGFW_mouseArrow,
        RGFW_mousePointingHand,
        RGFW_mouseIbeam,
        RGFW_mouseCrosshair,
        RGFW_mouseResizeAll, // @todo? RGFW_mouseResizeEW,RGFW_mouseResizeNS,RGFW_mouseResizeNWSE,RGFW_mouseResizeNESW,
        RGFW_mouseNotAllowed,
    };
    static int last = 0;
    if( mode != last )
    if( mode >= 0 && mode < COUNTOF(modes) ) {
        RGFW_window_showMouse(app_handle(), mode > 0);
        if( mode > 0 ) RGFW_window_setMouseStandard(app_handle(), modes[mode]);
        return last = mode;
    }
    return last;
}

void input_begin_(void *arg) {
    mouse_x_[1] = mouse_x_[0];
    mouse_y_[1] = mouse_y_[0];

    mouse_wheel = 0;
    up[up_count = 0] = down[down_count = 0] = text[text_count = 0] = 0;

    for( RGFW_event event; RGFW_window_checkEvent(app_handle(), &event); ) {
        switch (event.type) {
            default: break;
            break; case RGFW_mouseButtonReleased: if( event.button.value <= RGFW_mouseRight ) up[  up_count++] = event.button.value;
            break; case RGFW_mouseButtonPressed:  if( event.button.value <= RGFW_mouseRight ) down[down_count++] = event.button.value;
            break; case RGFW_keyReleased:           up[  up_count++] = event.key.value;
            break; case RGFW_keyPressed:          down[down_count++] = event.key.value;
                                                  text[text_count++] = event.key.sym;
        }
        switch (event.type) {
            default: break;
            case RGFW_quit: return;

            case RGFW_mousePosChanged:
                mouse_x_[0] = event.mouse.x, mouse_y_[0] = event.mouse.y;
                break;
            case RGFW_mouseButtonPressed:
            case RGFW_mouseButtonReleased:
                /**/ if( event.button.value == RGFW_mouseScroll ) mouse_wheel = event.scroll.y * 100;
                else if( event.button.value <= RGFW_mouseRight  ) mouse_button[ event.button.value ] = event.type == RGFW_mouseButtonPressed;
                break;

            case RGFW_keyPressed:
            case RGFW_keyReleased:
                keyboard[ event.key.value & 0xff ] = event.type == RGFW_keyPressed;
                break;
        }
    }
}

AUTORUN {
    on_frame_begin(input_begin_, NULL);
}
#endif
