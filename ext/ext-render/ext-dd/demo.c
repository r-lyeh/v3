#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    app_create(.title = "v3(ddraw)", .scale = 0.75);

    camera_t cam = { .pos=vec3(12,16,30), .yaw=-116, .pitch=-25 };

    while( app_swap() ) {
        vec3 offset = vec3(key('d')-key('a'),key('e')-key('q')-key('c'),key('w')-key('s'));
        camera_offset(&cam, offset, key(' ') ? 1.5 : 0.5);

        vec3 turn = vec3(mouse('u'), -mouse('v'), 0);
        camera_turn(&cam, turn, mouse('l') * !ui_hovered() * (key(' ') ? 1.5 : 0.5));

        if( ui_tree("Camera") ) {
            ui_vec3("Yaw,Pitch,Roll", &cam.ypr.x, -180, 180);
            ui_vec3("Position", &cam.pos.x, -1000, 1000);
            ui_tree_end();
        }

        // draw floor
        dd_grid();

        // two orange lines crossed. one of them is not anti-aliased
        dd_line(vec3(10,10,10), vec3(-10,10,-10), .color = V3_ORANGE);
        dd_line(vec3(-10,10,10), vec3(10,10,-10), .color = V3_ORANGE, .aa=0);

        // batch 10 points starting at 10,10,10
        dd_point(vec3(10,10,10*sin(time_ss())), .color = V3_PURPLE, .count = 10, .incpos = vec3(1,2,3));

        // draw cameras which mimic our camera rotation
        dd_camera(vec3(0,10,20), .fwd = cam.fwd, .up = cam.up, .color = V3_GREEN);

        // bunch of other stuff
        static int show_demo = 1;
        if( show_demo ) dd_demo();

        // flush commands to screen
        mat44 proj = proj44(45);
        mat44 view = view44(cam);
        dd_flush(proj, view);

        // ui
        ui_bool("DebugDraw demo", &show_demo);
        ui_label(va("%5.2f/%5.2f fps, %5.2f ms/f", fps(), hz(), spf()*1e3));
    }

    return 0;
}
