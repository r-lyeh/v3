#ifndef ARCHIVE_DIR_H
#define ARCHIVE_DIR_H

#include "3rd_dir.h"

// @todo: md() recursive mkdir
// @todo: rd() recursive rmdir
extern char **(*ls)(const char *path, int *count); // do not free()
extern char **(*lsr)(const char *path, int *count); // do not free()

#ifdef PATH_MAX
enum { DIR_MAX = PATH_MAX };
#elif defined MAX_PATH
enum { DIR_MAX = MAX_PATH };
#else
enum { DIR_MAX = 260 };
#endif

#elif CODE
#pragma once

#define DIR_C
#include "3rd_dir.h"

#include v3_ls_backend
char **(*ls)(const char *path, int *count) = ls_;
char **(*lsr)(const char *path, int *count) = lsr_;

#if 0
UNITTEST {
    int count;
    char **list = ls(".", &count);
    test( list );
    test( count > 0 );
    test( list[0] );
    test( list[count] == 0 );
    //for( int i = 0; i < count; ++i ) puts(list[i]);
}
#endif

#endif
