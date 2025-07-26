#define V3_IMPLEMENTATION
#include "v3.h"

typedef struct my_sprite {
    vec3 pos;
    vec3 size;
} my_sprite, my_sprite_t;

var sprite_name() { return var(.str = "my_sprite"); }
var sprite_edit(my_sprite *s) { return var(.i = vec3_edit(&s->pos).i | vec3_edit(&s->size).i); }

AUTORUN {
    editor_add_method("my_sprite", NAME, sprite_name);
    editor_add_method("my_sprite", EDIT, sprite_edit);
}

// @todo: move this to a dll apart
unsigned game(unsigned frame, float dt, mat44 proj, mat44 view) {

    if( frame == 0 ) {
        // @todo: restart game state
    }

    // render game to a texture
    static rendertarget_t r; ONCE r = rendertarget();
    rendertarget_use(&r, app_width(), app_height());

        glClear(GL_COLOR_BUFFER_BIT);

        dd_demo();
        dd_flush(proj, view);

    rendertarget_use(NULL, 0, 0);

    return r.color_texture;
}

AUTORUN {
    editor my_window = editor(.title = "Game view");
    editor_push(my_window);
}

int main() {

    editor_dump_class(stdout, "*");

    unsigned frame = 0, speed = 0;
    camera_t cam = { .pos=vec3(12,16,30), .yaw=-116, .pitch=-25 };

    for( app_create(.title="v3(editor)", .scale=100, .transparent=1); app_swap() && !key(0); frame += speed ) {

        vec3 offset = vec3(key('d')-key('a'),key('e')-key('q')-key('c'),key('w')-key('s'));
        camera_offset(&cam, offset, key(' ') ? 1.5 : 0.5);

        vec3 turn = vec3(mouse('u'), -mouse('v'), 0);
        camera_turn(&cam, turn, (mouse('l')|mouse('r')) * !ui_hovered() * (key(' ') ? 1.5 : 0.5));

        // step time
        static uint64_t then = 0;
        uint64_t now = time_ns();
        int64_t dt = then - now;
        then = now; ONCE dt = 0;

        // render game into a texture
        unsigned color_texture = game(frame, dt / 1e9, proj44(45), view44(cam));

        // render all viewports
        editor_tick();
        editor_draw(color_texture);

        // headless ui
        if( ui_label(va("%5.2ffps, %5.2fmspf, %d frame", fps(), spf() * 1000, frame))) {}

        // @todo: eject, slomo/fast-fwd >> 0.125,0.250,0.5,0,1,2,4,8
        int choice = ui_buttons(5, "load", "save", speed ? "hold" : "play", "stop", "quit");
        if( choice == 3 ) { if( frame > 0 ) speed ^= 1; else speed = 1; }
        if( choice == 4 ) frame = 0, speed = 0;
        if( choice == 5 ) break;

        if( ui_tree("Camera") ) {
            ui_vec3("Yaw,Pitch,Roll", &cam.ypr.x, -180, 180);
            ui_vec3("Position", &cam.pos.x, -1000, 1000);
            ui_tree_end();
        }

        // test reflection

        static vec3 v = {1,2,3};
        class_t c = editor_class("vec3");
        ui_label(c.NAME(&v).str);
        c.EDIT(&v);

        if( ui_window("spr", 0) ) {
            static my_sprite spr = { {4,5,6}, {7,8,9} };
            editor_class("my_sprite").EDIT(&spr);
            ui_window_end();
        }
    }

    return 0;
}
