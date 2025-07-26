#if CODE
#pragma once

int mouse_x[2], mouse_y[2], mouse_wheel, mouse_button[3];

int mouse_(int vk) {
    static const int* addr[] = {
        ['x'] = &mouse_x[0], ['y'] = &mouse_y[0], ['w'] = &mouse_wheel,
        ['l'] = &mouse_button[0], ['r'] = &mouse_button[1], ['m'] = &mouse_button[2],
        ['0'] = &mouse_button[0], ['1'] = &mouse_button[1], ['2'] = &mouse_button[2],
        [ 0 ] = &mouse_button[0], [ 1 ] = &mouse_button[1], [ 2 ] = &mouse_button[2],
    };
    if( vk == 'u' ) return mouse_x[0] - mouse_x[1];
    if( vk == 'v' ) return mouse_y[0] - mouse_y[1];
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

void mouse_begin_(void *arg) {
    mouse_x[1] = mouse_x[0];
    mouse_y[1] = mouse_y[0];

    mouse_wheel = 0;
    up[up_count = 0] = down[down_count = 0] = 0;

    RGFW_window *handle = app_handle();
    for( RGFW_event event; RGFW_window_checkEvent(handle, &event); ) {
        switch (event.type) {
            default: break;
            break; case RGFW_mouseButtonReleased: if( event.button.value <= RGFW_mouseRight ) up[  up_count++] = event.button.value + 1;
            break; case RGFW_mouseButtonPressed:  if( event.button.value <= RGFW_mouseRight ) down[down_count++] = event.button.value + 1;
        }
        switch (event.type) {
            default: break;
            case RGFW_quit: return;

            case RGFW_mousePosChanged:
                mouse_x[0] = event.mouse.x, mouse_y[0] = event.mouse.y;
                break;
            case RGFW_mouseButtonPressed:
            case RGFW_mouseButtonReleased:
                /**/ if( event.button.value == RGFW_mouseScroll ) mouse_wheel = event.scroll.y * 100;
                else if( event.button.value <= RGFW_mouseRight  ) mouse_button[ event.button.value ] = event.type == RGFW_mouseButtonPressed;
                break;
        }
    }
}

AUTORUN {
    on_frame_begin(mouse_begin_, NULL);
}
#endif
