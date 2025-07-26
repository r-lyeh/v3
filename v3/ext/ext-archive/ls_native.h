//#define dir dir2
#define DIR_C
#include "3rd_dir.h"

#define string char*

static void dir_deleter_(void *ptr) {
    string*list = (string*)ptr;
    for( int i = 0; list[i]; ++i )
        free( list[i] );
}

static string* ls_ex(const char *path, int *count, int recurse) {
    int numdirs = 0;
    string *list = 0;
    if( path )
    for( dir_t *d = dir_open(path, recurse ? "r" : ""); d; dir_close(d), d = 0) {
        numdirs = dir_count(d);
        list = make( (numdirs+1) * sizeof(string), dir_deleter_ );
        for( int i = 0; i < numdirs; ++i ) {
            list[i] = strdup(dir_name(d, i));
        }
        list[numdirs] = 0;
    }
    if( count ) *count = numdirs;
    return list;
}

string* ls_(const char *path, int *count) {
    return ls_ex(path, count, 0);
}

string* lsr_(const char *path, int *count) {
    return ls_ex(path, count, 1);
}

#undef string
#undef dir
