extern int         (*argc)(void);
extern char*       (*argv)(int);
extern char*       (*argv0)(void); // argv0() returns clean argv[0]: no path + no .exe extension

#if CODE
#pragma once

#if V3_MACOS
#include <crt_externs.h>
#endif

#if V3_GCC && !V3_MINGW
int __argc; char **__argv;
ifdef(V3_WASM,,__attribute__((constructor)) void init_argcv(int c, char **v) { __argc = c; __argv = v; });
#endif

int argc_(void) {
    ifdef(V3_MACOS, ONCE __argc = *_NSGetArgc());
    return __argc;
}
char* argv_(int arg) {
    ifdef(V3_MACOS, ONCE __argv = *_NSGetArgv());
    static threadlocal_ char empty[1];
    return (unsigned)arg < argc_() ? __argv[arg] : (empty[0] = '\0', empty);
}
char *argv0_(void) {
    static char *buf;
    ONCE {
        buf = strdup(argv_(0));

        // remove .exe suffix on windows
        #if _WIN32
        const char *found = strendi(buf, ".exe");
        if( found ) *((char*)found) = '\0';
        #endif

        // remove paths from argv[0]: ../home/prj/my_app > my_app
        char *a = strrchr(buf, '/');
        char *b = strrchr(buf, '\\');
        buf = a > b ? a+1 : b > a ? b+1 : buf;
    }
    return buf;
}

int  (*argc)(void) = argc_;
char*(*argv)(int) = argv_;
char*(*argv0)(void) = argv0_;

#endif // CODE
