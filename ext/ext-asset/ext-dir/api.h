#include "3rd/dir.h"

// @todo: md() recursive mkdir
// @todo: rd() recursive rmdir
extern char **(*ls)(const char *path, int *count); // do not free()
extern char **(*lsr)(const char *path, int *count); // do not free()

#if CODE
#pragma once
#define DIR_C
#include "3rd/dir.h"
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
