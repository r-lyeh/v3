#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    app_create();

    camera_t cam = { .pos=vec3(12,16,30), .yaw=-116, .pitch=-25 };

    // Create FBO and texture
    rendertarget_t f = rendertarget();

    // Main loop
    while (app_swap()) {
        vec3 offset = vec3(key('d')-key('a'),key('e')-key('q')-key('c'),key('w')-key('s'));
        camera_offset(&cam, offset, key(' ') ? 1.5 : 0.5);

        vec3 turn = vec3(mouse('u'), -mouse('v'), 0);
        camera_turn(&cam, turn, mouse('l') * !ui_hovered() * (key(' ') ? 1.5 : 0.5));

        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Clear FBO texture to red
        rendertarget_use(&f, app_width(), app_height());
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        dd_demo();
        dd_flush(proj44(45),view44(cam));
        rendertarget_use(NULL, 0, 0);

        // Render FBO texture to screen
        sprite(f.color_texture, mouse('x'), mouse('y'), app_width()/2, app_height()/2);
    }

    // Cleanup
    rendertarget_destroy(&f);
    return 0;
}
