#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    for( app_create(.title = "gamepad", "0.50"); app_swap(); ) {
        ui_label(va("%d gamepads connected", gamepad_count()));
        for( int id = 0, max = gamepad_count(); id < max; ++id ) {
            ui_label(va("[%d] Battery: %3f%%", id, gamepad_live(id)));

            #define DO_UI(x) ui_label(va("[%d] %s: %f,%f,%f,%f", id, #x-(-8), gamepad_read(id,x), gamepad_idle(id,x),gamepad_down(id,x),gamepad_held(id,x)));

            GAMEPAD_FOREACH(DO_UI)

            #undef DO_UI
        }
    }
    return 0;
}
