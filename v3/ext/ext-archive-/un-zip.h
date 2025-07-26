#define ZIP_C
#include "3rd_zip.h"

// zipfilepath` syntax: "c:/prj/my.zip/mydir/myfile"

bool zipme(const char *fullpath, const char *bin, int len) {
    const char *filename = strstr(fullpath, ".zip/"); filename += !!filename * 5;
    if( !filename ) return false;
    const char *zipfile = va("%.*s", (int)(filename - 1 - fullpath), fullpath);

    struct zip *z = zip_open(zipfile, "a+b");
    if( z ) {
        unsigned compress_level = 9;
        const char *entryname = filename;
        const char *comment = "";
        bool ok = zip_append_mem(z, entryname, comment, bin, len, compress_level);
        zip_close(z);
        return ok;
    }

    return false;
}

char *unzip_ex(const char *fullpath, int *len, const char **url) { // must free() rc after use. do not free url.
    if( url ) *url = 0;
    if( len ) *len = 0;
    const char *filename = strstr(fullpath, ".zip/"); filename += !!filename * 5;
    if( !filename ) return NULL;
    const char *zipfile = va("%.*s", (int)(filename - 1 - fullpath), fullpath);

    char *bin = 0;
    struct zip *z = zip_open(zipfile, "rb");
    if( z ) {
        int index = -1;
        if( strchr(filename, '*') ) {
            for( unsigned i = 0, end = zip_count(z); i < end; ++i ) {
                if( strmatchi(zip_name(z,i), filename) ) {
                    index = i;
                }
            }
        }
        else if( strchr(filename, '@') ) {
            unsigned i = atoi(strchr(filename, '@')+1);
            if( i < zip_count(z) ) {
                index = i;
            }
        }
        else {
            index = zip_find(z, filename);
        }
        if( index >= 0 ) {
            bin = zip_extract(z, index);
            if(len) *len = zip_size(z, index);
            if(url) *url = va("%s", zip_name(z, index));
        }
        zip_close(z);
    }

    return bin;
}

char *unzip(const char *fullpath, int *len) { // must free() after use
    return unzip_ex(fullpath, len, NULL);
}
