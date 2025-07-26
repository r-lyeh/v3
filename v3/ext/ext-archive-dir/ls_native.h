#define string char*

static string* ls_ex(const char *path, int *count, int recurse) {
    static THREAD string* lists[16] = {0};
    static THREAD int slot = 0;
    string **list = &lists[ slot = (slot+1) % COUNTOF(lists) ];

    int numdirs = 0;
    if( path )
    for( dir_t *d = dir_open(path, recurse ? "r" : ""); d; dir_close(d), d = 0) {
        numdirs = dir_count(d);
        *list = resize( *list, (numdirs+1) * sizeof(string) );
        for( int i = 0; i < numdirs; ++i ) {
            const char *dirname = dir_name(d, i);
            int dirnamelen = strlen(dirname) + 1;
            memcpy( (*list)[i] = resize((*list)[i], dirnamelen), dirname, dirnamelen);
        }
        (*list)[numdirs] = 0;
    }
    if( count ) *count = numdirs;
    return *list;
}

string* ls_(const char *path, int *count) {
    return ls_ex(path, count, 0);
}

string* lsr_(const char *path, int *count) {
    return ls_ex(path, count, 1);
}

#undef string
