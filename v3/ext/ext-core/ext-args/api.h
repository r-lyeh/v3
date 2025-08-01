extern int         (*argc)();
extern char*       (*argv)(int); // argv(0) returns no path + no .exe extension

#if CODE
#pragma once

#if DEFINED(gcc) && !DEFINED(mingw)
int __argc; char **__argv;
ifdef(ems,,__attribute__((constructor)) void init_argcv(int c, char **v) { __argc = c; __argv = v; });
#endif

int argc_() {
    return __argc;
}
char* argv_(int arg) {
    ONCE {
        // remove .exe suffix on windows
        #if _WIN32
        const char *found = strendi(__argv[0], ".exe");
        if( found ) *((char*)found) = '\0'; // ok __argv is writeable
        #endif

        // remove paths from argv[0]: ../home/prj/my_app > my_app
        char *a = strrchr(__argv[0], '/');
        char *b = strrchr(__argv[0], '\\');
        __argv[0] = a > b ? a+1 : b > a ? b+1 : __argv[0];
    }
    static THREAD char empty[1];
    return (unsigned)arg < __argc ? __argv[arg] : (empty[0] = '\0', empty);
}

int  (*argc)() = argc_;
char*(*argv)(int) = argv_;

#endif // CODE
