#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    // create context with 8x MSAA max
    app_create(.title="v3(msaa)", .aa = 8);

    camera_t cam = { .pos=vec3(12,16,30), .yaw=-116, .pitch=-25 };

    while (app_swap()) {
        vec3 offset = vec3(key('d')-key('a'),key('e')-key('q')-key('c'),key('w')-key('s'));
        camera_offset(&cam, offset, key(' ') ? 1.5 : 0.5);

        vec3 turn = vec3(mouse('u'), -mouse('v'), 0);
        camera_turn(&cam, turn, mouse('l') * !ui_hovered() * (key(' ') ? 1.5 : 0.5));

        if( ui_tree("Camera") ) {
            ui_vec3("Yaw,Pitch,Roll", &cam.ypr.x, -180, 180);
            ui_vec3("Position", &cam.pos.x, -1000, 1000);
            ui_tree_end();
        }

        // Select MSAA level (e.g., from user input or config)
        static int level = 1; // Start with 1x for speed
        if (key_down('0')) level = 0;
        if (key_down('1')) level = 1;
        if (key_down('2')) level = 2;
        if (key_down('4')) level = 4;
        if (key_down('8')) level = 8;
        ui_label(va("FBO MSAA: %d", level));
        ui_int("FBO MSAA", &level, 1, 8);

#if 1
        // Bind FBO or default framebuffer
        msaa_begin(level);

        // Clear and render your scene
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

        // draw anything
        dd_demo(); // dd_grid()

        // flush commands to screen
        mat44 proj = proj44(45);
        mat44 view = view44(cam);
        dd_flush(proj, view);

#if 1
        // Blit to default framebuffer (skipped if level == MSAA_OFF || level >= CONTEXT_AA)
        msaa_end();
#endif
    }

    // Cleanup
    return 0;
}
