// downloads url. returns associated local file. or null if could not download.
const char *url(const char *u);

// same than above, but does file caching. if fstamp > expire date, downloads again.
const char *urlcache(const char *u, unsigned expires);

#if CODE
#pragma once
#include v3_download_backend

const char *url(const char *u) {
    const char *pathfile = va(".%p.url", (void*)(uintptr_t)fnv1a(u, strlen(u)));
    int st = path_stat(pathfile), len;
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

#endif
