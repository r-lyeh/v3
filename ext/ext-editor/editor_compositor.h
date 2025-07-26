// Basically, the editor is a mostly empty app, that composites multiple docked windows.
// - The editor collects user inputs, which are sent to focused windows.
// - Each window decides how to interpret user inputs and what to render from them.
// - Each window sends back a rendered texture to the editor, which composes the final picture.
//
//    editor ------> which reads inputs ---------> which are sent to window
//    ^--- which is displayed by <---------- which generates texture <----v
//
// editor = for each window[] : blit(.texture = window.draw(user.inputs))

void editor_tick();
void editor_draw(unsigned game_texture_id);

#if CODE
#pragma once

#if V3_RETAIL

void editor_tick() {}
void editor_draw(unsigned game_texture_id) {}

#else

void editor_tick() {
    // step time
    static uint64_t then = 0;
    uint64_t now = time_ns();
    int64_t dt = then - now;
    then = now; ONCE dt = 0;

    for(int i = 0, e = array_count(editors); i < e; ++i) {
        editor_t *app = &editors[i];

        if( app->deleted ) continue;
        if( !app->open ) continue;

        if( app->vt.ON_TICK ) app->vt.ON_TICK();

        if( !app->focus ) continue;
        if( app->vt.ON_BIND ) app->vt.ON_BIND();
    }
}

void editor_draw(unsigned game_texture_id) {
    void *texture_handle = (void*)(uintptr_t)game_texture_id;

    for( int i = 0, e = array_count(editors); i < e; ++i ) {

        editor_t *app = &editors[i];
        if( app->deleted ) continue;
        if( !app->open ) continue;

        if( ui_window(app->title, 0) ) {

            ui_texture( texture_handle );
            if( app->vt.ON_DRAW ) app->vt.ON_DRAW( texture_handle );

            ui_window_end();
        }
    }

    int shortcut = 1;
    for( int i = 0, e = array_count(editors); i < e; ++i ) {
        editor_t *app = &editors[i];
        if( app->deleted ) continue;

        if( ui_button(va("%d%s", shortcut, app->open ? "*":"")) ) {
            app->open ^= 1;
            if(app->open) {
                extern mu_Context *ui_ctx;
                mu_Container *cnt = mu_exists(ui_ctx, app->title);
                if(cnt) cnt->open = 1;
                if(cnt) mu_bring_to_front(ui_ctx, cnt);
            }
        }

        shortcut++;
    }
}

#endif

#endif
