// demo launcher. invoked as:
//
//   v3
//   v3 [number]       ;; can be a partial match
//   v3 [appname]      ;; can be a partial match
//
// compiled via `./v3.sh`, or manually:
//
//   echo linux        && cc v3.c -lm -ldl -lpthread -lasound
//   echo win/vc       && cl v3.c                   
//   echo win/clang    && clang v3.c                
//   echo win/clang-cl && clang-cl v3.c             
//   echo win/mingw    && gcc v3.c -lgdi32 -ldbghelp -lole32 -lwininet
//   echo osx          && SDKROOT=macosx10.14 cc -ObjC v3.c -framework cocoa -framework iokit -framework audiotoolbox

// compile v3 as dll:
//
//   cl v3.c /LD /MT                         ; see `dumpbin /exports v3.dll`
//

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

void help() {
    puts("Usage:");
    for( int demo = 1; demo <= demos_num; ++demo ) {
        const char *path = demos[demo-1].path;
        const char *name = strrstr(path, "ext"); // "ext/"
              char *buf = va("%s", name ? name + 4 : path);
        replace(buf, "/demo-", "/");
        replace(buf, "/demo", "/");
        replace(buf, "/.c", "");
        replace(buf, ".c", "");
        printf("%s [%d][%s]\n", argv0(), demo, buf);
    }
}

int main() {
    qsort(demos, demos_num, sizeof(demos[0]), app_cmp);

#ifdef AUTORUN
    if( argc() > 1 ) {
        if( !strcmp(argv(1), "sync") || !strcmp(argv(1), "tidy") || !strcmp(argv(1), "ext") ) {
            char cmdline[4096] = ifndef(V3_WINDOWS, "sh ") "v3.bat ";
            for( int i = 1; i < argc(); ++i ) strcat(cmdline, argv(i)), strcat(cmdline, " ");
            return system(cmdline);
        }
        for( int demo = 1; demo <= demos_num; ++demo )
            if( demo == atoi(argv(1)) || ( isalpha(argv(1)[0]) && strstri(demos[demo-1].path, argv(1)) ) )
                return demos[demo-1].main();
        return help(), -1;
    }

#if defined TEST || defined TESTS
    exit(-failures());
#endif

    if( demos_num == 1 ) {
        return demos[0].main();
    }

    // launch gui if launched from [explorer,finder,nautilus,etc]
    if( argv(0)[0] == '/' || argv(0)[1] == ':' )
    for( int run = (app_create(.title = "hello v3", .scale=100, .transparent=1), -1); app_swap(.color=0) && !key(0); ) {
        if( ui_window("hello v3", 0) ) {
            for( int i = 1; i <= demos_num; ++i ) {
                const char *appname = demos[i-1].path;
                const char *extname = strrstri(appname, "ext-");
                if(!extname)appname = path_base(appname);
                else appname = path_base(replace(replace(path_unix(extname), "/", " "), " demo", ""));

                int choice = ui_buttons(2, appname, "...");
                if( choice == 1 ) run = i;
                if( choice == 2 ) system(va("%s \"%s\"", ifdef(V3_WINDOWS,"start \"\"",ifdef(V3_MACOS,"open","xdg-open")), demos[i-1].path));
            }
            ui_window_end();
        }
        else return 0;
        if( run >= 0 && !mouse('l') ) app_show(0), system(va(ifndef(V3_WINDOWS, "./") "%s %d", argv0(), run)), app_show(1), run = -1;
    }

    // else cmdline version
    help();

#endif // AUTORUN

    return -1;
}

#pragma warning(disable : 4716)
#define main            ma1n( __FILE__, JOIN(app,__COUNTER__) )
#define ma1n(file,func) static MACRO(x); int func(); AUTORUN { demos[demos_num++] = ((demo_t){file, func}); } int func

#if __has_include("docs/index")
#         include "docs/index"
#endif
#if __has_include("ext/ext-demos.c")
#         include "ext/ext-demos.c"
#endif
#if __has_include("ext/ext-tests.c")
#         include "ext/ext-tests.c"
#endif
