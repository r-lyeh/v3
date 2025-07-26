#define string char*

static string* ls_ex(const char *path, int *count, int recurse) {
    static threadlocal_ string* list[16] = {0};
    static threadlocal_ int slot = 0; slot = (slot+1) % COUNTOF(list);

    int numdirs = 0;
    if( path )
    for( dir_t *d = dir_open(path, recurse ? "r" : ""); d; dir_close(d), d = 0) {
        numdirs = dir_count(d);
        list[slot] = realloc( list[slot], (numdirs+1) * sizeof(string) );
        for( int i = 0; i < numdirs; ++i ) {
            const char *dirname = dir_name(d, i);
            int dirnamelen = strlen(dirname) + 1;
            memcpy( list[slot][i] = realloc(list[slot][i], dirnamelen), dirname, dirnamelen);
        }
        list[slot][numdirs] = 0;
    }
    if( count ) *count = numdirs;
    return list[slot];
}

string* ls_(const char *path, int *count) {
    return ls_ex(path, count, 0);
}

string* lsr_(const char *path, int *count) {
    return ls_ex(path, count, 1);
}

#undef string
