extern const char *(*cwd)(void);    // get current working directory
extern const char *(*cwdexe)(void); // get executable working directory

#if CODE
#pragma once
#include ifdef(V3_WINDOWS, <direct.h>, <unistd.h>)
#define getcwd_ ifdef(V3_WINDOWS, _getcwd, getcwd)

const char *cwd_(void) {
    static char buf[FILENAME_MAX] = {0};
    getcwd_(buf, sizeof(buf));
    if( !strchr("\\/", buf[strlen(buf)-1]) ) strcat(buf, ifdef(V3_WINDOWS,"\\","/"));
    return buf;
}

const char* cwdexe_(void) {
    static char buffer[FILENAME_MAX] = {0};
    if( !buffer[0] ) {
        // find executable folder
        #if defined _WIN32
        GetModuleFileNameA(0,buffer,FILENAME_MAX);
        *strrchr(buffer, '\\') = '\0';
        #elif defined __APPLE__
        realpath(argv0(),buffer);
        if(strrchr(buffer, '/')) 1[strrchr(buffer,'/')] = '\0';
        #else // linux
        char path[32] = {0};
        sprintf(path, "/proc/%d/exe", getpid());
        readlink(path, buffer, sizeof(buffer));
        if(strrchr(buffer, '/')) 1[strrchr(buffer,'/')] = '\0';
        #endif
    }
    return buffer;
}

AUTORUN {
    // relocate cwd to executable location
    ifdef(V3_WINDOWS, SetCurrentDirectoryA, chdir)(cwdexe_());
}

const char *(*cwd)(void) = cwd_;
const char *(*cwdexe)(void) = cwdexe_;

#endif
