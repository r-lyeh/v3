#include "3rd/zip.h"

#if CODE
#pragma once
#define ZIP_C
#include "3rd/zip.h"
#endif


#if 0

const char *normpath(const char *path) {
    if( !path[0] ) return "";

    static char result[PATH_MAX]; result[0] = '\0';

    // copy
    strncpy(result, path, PATH_MAX);
    // normalize
    for( int i = 0; result[i]; ++i ) if(result[i] == '\\') result[i] = '/';
    // no double slashes. use memmove (strcpy on same string is UB)
    for( int i = 0; result[i]; ++i ) {
        while( result[i] == '/' && result[i+1] == '/' )
            memmove(result+i, result+i+1, strlen(result+i+1)+1);
    }
    // if path points to a folder, mark it as such
    if( result[strlen(result)-1] != '/' && is_folder(result) )
        strcat(result, "/");

    return result;
}

const char *cmppath(const char *a, const char *b) { // returns byte in `a` where strings differ. NULL if strings are equal.
    while( *a && *b ) {
        int eq = *a == *b;
        if(eq) { ++a; ++b; continue; }
        eq = strchr("\\/", *a) && strchr("\\/", *b);
        if(eq) {
            do ++a; while(/**a &&*/ strchr("\\/", *a));
            do ++b; while(/**b &&*/ strchr("\\/", *b));
            continue;
        }
        #ifdef _WIN32
        eq = toupper(*a) == toupper(*b);
        if(eq) { ++a; ++b; continue; }
        #endif
        return a;
    }
    while(/**a &&*/ strchr("\\/", *a)) ++a;
    while(/**b &&*/ strchr("\\/", *b)) ++b;
    return *a == *b ? NULL : a;
}

const char *abspath(const char *rel_path) { // convert relative path to absolute. do not free()
    if( !rel_path[0] ) return "";

    static char result[PATH_MAX]; result[0] = '\0';
    realpath(rel_path, result);
    return normpath(result);
}

const char *relpath(const char *abs_path, const char *cwd) { // convert absolute path to relative. do not free()
    if( !abs_path[0] ) return "";

    static char result[PATH_MAX]; result[0] = '\0';

    // handle cases where paths are indentical or subfolders
    const char *mismatch = cmppath(abs_path, cwd);
    if( !mismatch ) {
        strcpy(result, "./");
    }
    else if( mismatch > abs_path ) {
        strcpy(result, "./");
        strcat(result, mismatch);
    }
    else {
        // find common prefix
        unsigned last_slash = 0;
        for( unsigned i = 0; cwd[i] && cwd[i] == abs_path[i]; ++i ) {
            if( strchr("/\\", abs_path[i]) ) last_slash = i;
        }
        int up_levels = 0;
        for( unsigned i = last_slash; cwd[i]; i++ ) {
            up_levels += !!strchr("/\\", cwd[i]);
        }

        // if no common prefix (different drives on Windows), return absolute path
        if( last_slash == 0 ) {
            strcpy(result, abs_path); // @fixme: should do differently?
        }
        else {
            // build the relative path
            for( int i = 0; i < up_levels; i++ ) {
                strcat(result, "../");
            }

            strcat(result, abs_path + last_slash + 1);
        }
    }

    return normpath(result);
}

const char *cwd() {
    static char buf[PATH_MAX] = {0};
    return !buf[0] ? getcwd(buf, sizeof(buf)) : buf;
}

#endif
