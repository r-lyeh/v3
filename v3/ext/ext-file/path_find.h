// reverse string (abc -> cba)
static char *strreverse(char *s) {
    int len = strlen(s);
    for( int i = 0; i < len/2; ++i ) {
        char x = s[i];
        s[i] = s[len - 1 - i];
        s[len - 1 - i] = x;
    }
    return s;
}

// list of scanned files
static char **manifest; int manifest_idx;

// scan subfolders and files and sort them into manifest list
static void scan(const char *folder) {
    for( dir_t *d = dir_open(folder, "r"); d; dir_close(d), d = 0 ) {
        manifest = realloc(manifest, sizeof(char*) * (dir_count(d)+1) );
        manifest[dir_count(d)] = 0;
        for( unsigned i = 0; i < dir_count(d); ++i ) {
            manifest[i] = strreverse
                (strdup(dir_name(d,i)));
        }
        qsort(manifest, manifest_idx = dir_count(d), sizeof(char*), strcmpq);
    }
}

// look up for partial leading match in manifest. case insensitive
char *path_find_(const char *file) { // file.txt > ./art/dir/file.txt
    ONCE scan("./");

    // reverse search key, since entries in our list are reversed
    char *key = strreverse(va("%s", file));

    // binary search: find key in list. leading match + case insensitive
    int kl = strlen(key);
    int min = 0, max = manifest_idx, found = -1;
    while( min <= max ) {
        int mid = min + ( max - min ) / 2;
        int search = strncmpi(key, manifest[mid], kl);
        /**/ if( search < 0 ) max = mid - 1;
        else if( search > 0 ) min = mid + 1;
        else { found = mid; break; }
    }

    return found >= 0 ? strreverse(va("%s", manifest[found])) : strreverse(key);
}
