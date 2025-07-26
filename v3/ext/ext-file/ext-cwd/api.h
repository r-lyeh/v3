extern const char *(*cwd)(void);
extern const char *(*cwdexe)(void);

#if CODE
#pragma once

#ifdef _WIN32
#include <direct.h>
#define GETCWD _getcwd
#else
#include <unistd.h>
#define GETCWD getcwd
#endif

const char *cwd_(void) {
    static char buf[DIR_MAX] = {0};
    GETCWD(buf, sizeof(buf));
    if( !strchr("\\/", buf[strlen(buf)-1]) ) strcat(buf, ifdef(win32,"\\","/"));
    return buf;
}

const char* cwdexe_(void) {
    static char buffer[MAX_PATH] = {0};
    if( !buffer[0] ) {
        // find executable folder
        #if defined _WIN32
        GetModuleFileName(0,buffer,MAX_PATH);
        *strrchr(buffer, '\\') = '\0';
        #elif defined __APPLE__
        realpath(argv(0),buffer);
        if(strrchr(buffer, '/')) 1[strrchr(buffer,'/')] = '\0';
        #else // linux
        char path[32] = {0};
        sprintf(path, "/proc/%d/exe", getpid());
        readlink(path, buffer, sizeof(buffer));
        if(strrchr(buffer, '/')) 1[strrchr(buffer,'/')] = '\0';
        #endif
        // change working dir to exe folder
        ifdef(win32, SetCurrentDirectoryA, chdir)(buffer);
    }

    // return cwd
    return cwd(); //buffer;
}

AUTORUN {
    // relocate cwd to executable location
    cwdexe_();
}

const char *(*cwd)(void) = cwd_;
const char *(*cwdexe)(void) = cwdexe_;

#endif
