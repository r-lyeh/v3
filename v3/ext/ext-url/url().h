#include "3rd/download.h"

const char *urlcache_(const char *u, uint64_t expired) {
    const char *pathfile = va(".%p.url", (void*)(uintptr_t)fnv1a(u, strlen(u)));
    int st = path_stat(pathfile), len;
    if( st == 1 ) if( path_date(pathfile) >= expired ) st = 0;
    if( st == 1 ) return pathfile;
    if( st == 0 ) {
        for( char *data = download(u, &len); data; free(data), data = 0 ) {
            if( file_save(pathfile, data, len) ) {
                return free(data), pathfile;
            }
        }
    }
    return NULL;
}

const char *url_(const char *u) {
    return urlcache_(u, 0);
}
