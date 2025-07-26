#define V3_IMPLEMENTATION
#include "../v3/v3.h"

int main() {
    app_create("ui demo", "0.85");

    while( app_swap() ) {
        // text
        ui_label(va("app(%d,%d) mouse(%d,%d)", app_width(), app_height(), mouse('x'), mouse('y')));

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
