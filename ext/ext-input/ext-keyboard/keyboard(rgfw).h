#if CODE
#pragma once

int up[RGFW_keyLast],   up_count = 0;   // keyboard ime
int down[RGFW_keyLast], down_count = 0; // keyboard ime
int text[RGFW_keyLast], text_count = 0; // keyboard ime
int keyboard[RGFW_keyLast];

int key_(int vk) {
    static const int keycodes[256] = { [0] = RGFW_escape };
    vk = tolower(vk);
    return RGFW_isKeyDown(vk); // keycodes[vk]); // RGFW_isKeyPressed
}

void keyboard_begin_(void *arg) {
    up[up_count = 0] = down[down_count = 0] = text[text_count = 0] = 0;

    RGFW_window *handle = app_handle();
    for( RGFW_event event; RGFW_window_checkEvent(handle, &event); ) {
        switch (event.type) {
            default: break;
            break; case RGFW_keyReleased:           up[  up_count++] = event.key.value;
            break; case RGFW_keyPressed:          down[down_count++] = event.key.value;
                                                  text[text_count++] = event.key.sym;
        }
        switch (event.type) {
            default: break;
            case RGFW_quit: return;

            case RGFW_keyPressed:
            case RGFW_keyReleased:
                keyboard[ event.key.value & 0xff ] = event.type == RGFW_keyPressed;
                break;
        }
    }
}

AUTORUN {
    on_frame_begin(keyboard_begin_, NULL);
}
#endif
