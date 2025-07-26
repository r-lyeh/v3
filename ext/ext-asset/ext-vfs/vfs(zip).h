static zip_t *mounted[256]; int mounted_idx;
static file_t (*fileread_prev)(const char *);

static file_t vfs_read(const char *fname) {
    // inspect disk files first
    file_t f = fileread_prev(fname);
    if( f.bin && f.len ) return f;

    // try mounted files after. @todo: implement binary search + partial matches (by using reversed keys)
    for( int j = 0; j < mounted_idx; ++j ) {
        zip_t *z = mounted[j];
        for( int i = 0, end = zip_count(z); i < end; ++i ) {
            if( !strcmp(zip_name(z,i), fname) ) {
                file_t f = {0};
                f.bin = zip_extract(z, i);
                f.len = zip_size(z, i);
                strncpy(f.url, fname, COUNTOF(f.url));
                return f;
            }
        }
    }
    return blob(0,0);
}

bool vfs_mount_(const char *media) {
    // mount appname + .zip file automatically
    ONCE vfs_mount_( va("%s%s.zip", cwdexe(), argv0()) );
    ONCE vfs_mount_( va("%s%s.zip", cwdexe(), ".cook") );

    assert(mounted_idx < COUNTOF(mounted));

    zip_t *z = zip_open(media, "r");
    if( z ) {
        mounted[mounted_idx++] = z;

        lputs(va("vfs_mount: mounted `%s`", media));
        return 1;
    }
    return 0;
}

AUTORUN {
    // keep previous loader safe and replace it
    fileread_prev = file_read, file_read = vfs_read;
}
