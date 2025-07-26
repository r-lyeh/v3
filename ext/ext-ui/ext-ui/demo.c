#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    app_create(.title = "ui demo", .scale = 0.75);

    while( app_swap() ) {
        // text
        ui_label(va("app(%d,%d) mouse(%d,%d) ui(%d)", app_width(), app_height(), mouse('x'), mouse('y'), ui_hovered()));

        // button
        if( ui_button("some other button") ) {
            puts("clicked too");
        }

        if( ui_window("window", 0) ) {
            if( ui_button("click") ) {
                puts("clicked");
            }
            ui_window_end();
        }
    }
    return 0;
}
