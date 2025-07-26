// demo launcher. invoked as:
//
//   v3
//   v3 [number]
//   v3 [appname]
//   v3 [/path/file.lua]
//
// compiled via `[sh] v3.bat`, or manually:
//
//   echo linux        && cc v3.c -lm -ldl -lpthread -lasound
//   echo win/vc       && cl v3.c                   
//   echo win/clang    && clang v3.c                
//   echo win/clang-cl && clang-cl v3.c             
//   echo win/mingw    && gcc v3.c -lgdi32 -ldbghelp -lole32 -lwininet
//   echo osx          && SDKROOT=macosx10.14 cc -ObjC v3.c -framework cocoa -framework iokit -framework audiotoolbox

#define V3_IMPLEMENTATION
#include "v3.h"

typedef struct demo_t {
    const char *path;
    int(*main)();
} demo_t;

demo_t demos[64]; int demos_num;

int app_cmp( const void *a, const void *b ) {
    return strcmp( ((demo_t*)a)->path, ((demo_t*)b)->path );
}

int main() {
    qsort(demos, demos_num, sizeof(demos[0]), app_cmp);

    if( argc() > 1 ) {
        if( !strcmp(argv(1), "sync") || !strcmp(argv(1), "tidy") || !strcmp(argv(1), "ext") ) {
            char cmdline[4096] = ifndef(V3_WINDOWS, "sh ") "v3.bat ";
            for( int i = 1; i < argc(); ++i ) strcat(cmdline, argv(i)), strcat(cmdline, " ");
            return system(cmdline);
        }
        if( strstr(argv(1), ".lua") || strstr(argv(1), ".tl") ) // if(isfile())
        for(script_t *L = script_create(); L; script_destroy(L), L = 0)
            script_runfile(L, argv(1));
        else
        for( int demo = 1; demo <= demos_num; ++demo )
            if( demo == atoi(argv(1)) || ( isalpha(argv(1)[0]) && strstri(demos[demo-1].path, argv(1)) ) )
                return demos[demo-1].main();
        return 0;
    }

    #if defined TEST || defined TESTS
    exit(-failures());
    #endif

    for( app_create("hello v3", "0.50"); app_swap() && !key('\x1b');  ) {
        if( ui_window("hello v3", 0) ) {
            for( int i = 1; i <= demos_num; ++i ) {
                const char *appname = demos[i-1].path;
                const char *extname = strrstri(appname, "ext-");
                if(!extname)appname = path_base(appname);
                else appname = path_base(replace(replace(path_unix(extname), "/", " "), " demo", ""));

                int choice = ui_buttons(2, appname, "...");
                if( choice == 1 ) app_show(0), system(va(ifndef(V3_WINDOWS, "./") "%s %d", argv(0), i)), app_show(1);
                if( choice == 2 ) system(va("%s \"%s\"", ifdef(V3_WINDOWS,"start \"\"",ifdef(V3_MACOS,"open","xdg-open")), demos[i-1].path));
            }
            ui_window_end();
        }
        else break;
    }
}

#pragma warning(disable : 4716)
#define main            ma1n( __FILE__, JOIN(app,__COUNTER__) )
#define ma1n(file,func) static MACRO(x); int func(); AUTORUN { demos[demos_num++] = ((demo_t){file, func}); } int func

#include "docs/index"
#include "ext/ext-demos.c"
#include "ext/ext-tests.c"
