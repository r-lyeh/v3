#if CODE
#pragma once

static
void cwdexe(void) {
    // relocate cwd to exe folder (relative paths may not work from this point)
    static char buffer[MAX_PATH] = {0};
    if( !buffer[0] ) {
        #if defined _WIN32
        GetModuleFileName(0,buffer,MAX_PATH);
        *strrchr(buffer, '\\') = '\0';
        #elif defined __APPLE__
        realpath(__argv[0],buffer);
        if(strrchr(buffer, '/')) 1[strrchr(buffer,'/')] = '\0';
        #else // linux
        char path[32] = {0};
        sprintf(path, "/proc/%d/exe", getpid());
        readlink(path, buffer, sizeof(buffer));
        if(strrchr(buffer, '/')) 1[strrchr(buffer,'/')] = '\0';
        #endif
    }

    ifdef(win32, SetCurrentDirectoryA, chdir)(buffer);
}

AUTORUN {
    // relocate cwd to executable location
    cwdexe();
}

#endif
