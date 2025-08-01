// demo launcher. can be invoked as:
//   hello
//   hello [number]
//   hello [appname]
//   hello [/path/file.lua]

#define V3_IMPLEMENTATION
#include "v3/v3.h"

typedef struct demo_t {
    const char *path;
    int(*main)();
} demo_t;

demo_t demos[64]; int demos_cnt;

int app_cmp( const void *a, const void *b ) {
    return strcmp( ((demo_t*)a)->path, ((demo_t*)b)->path );
}

int main() {
    qsort(demos, demos_cnt, sizeof(demos[0]), app_cmp);

    if( __argc > 1 ) {
        if( strstr(__argv[1], ".lua") || strstr(__argv[1], ".tl") ) // if(isfile())
        for(script_t *L = script_create(); L; script_destroy(L), L = 0)
            script_runfile(L, __argv[1]);
        else
        for( int demo = 0; demo < demos_cnt; ++demo )
            if( demo == atoi(__argv[1]) || ( isalpha(__argv[1][0]) && strstr(demos[demo].path, __argv[1]) ) )
                return demos[demo].main();
        return 0;
    }

    app_create("hello v3", 0.50);

    while( app_swap() && !key('\x1b') ) {
        if( ui_window("hello", 0) ) {
            for( int i = 0; i < demos_cnt; ++i ) {
                const char *app0 = strrchr(demos[i].path, '/');
                const char *app1 = strrchr(demos[i].path, '\\');
                const char *appname = app0 > app1 ? app0+1 : app1 > app0 ? app1+1 : demos[i].path;

                int choice = ui_buttons(2, appname, "...");
                if( choice == 1 ) system(va("%s %d", __argv[0], i));
                if( choice == 2 ) system(va("%s \"%s\"", ifdef(win32,"start \"\"",ifdef(osx,"open","xdg-open")), demos[i].path));
            }
            ui_window_end();
        }
        else break;
    }
}

#pragma warning(disable : 4716)
#define main            ma1n( __FILE__, JOIN(app,__COUNTER__) )
#define ma1n(file,func) static MACRO(x); int func(); AUTORUN { demos[demos_cnt++] = ((demo_t){file, func}); } int func

#include "docs/00-null.c"
#include "docs/01-embed.c"
#include "docs/02-window.c"
#include "docs/03-ui.c"
#include "docs/04-opengl.c"
