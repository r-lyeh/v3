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
        for( unsigned i = 0; i < dir_count(d); ++i ) {
            const char *name = dir_name(d,i);
            if(strstr(name, "/.")) continue; // exclude hidden folders and files
            char *slash = strrchr(name, '/');
            int is_folder = slash && !slash[1]; if(is_folder) continue; // exclude purely folders
#ifdef LOOKUP_DEBUG
            static int count = 0;
            printf("%s : %d\n", name, count++);
#endif
            char *copy = strdup(name);
            manifest[manifest_idx++] = strreverse(copy);
        }
        manifest[manifest_idx] = 0;
        qsort(manifest, manifest_idx, sizeof(char*), strcmpqi);
#ifdef LOOKUP_DEBUG
        for( int i = 0; i < manifest_idx; ++i ) {
            printf("%s : %d\n", manifest[i], i);
        }
#endif
    }
}

// look up for partial leading match in manifest. case insensitive
char *lookup_(const char *file) { // file.txt > ./art/dir/file.txt
    ONCE scan("./");

    // reverse search key, since entries in our list are reversed
    char *key = strreverse(va("%s", file));

    // binary search: find key in list. leading match + case insensitive
    int kl = strlen(key);
    int min = 0, max = manifest_idx - 1, found = -1;
    if( max > 0 ) while( min <= max ) {
        int mid = min + ( max - min ) / 2;
        int search = strncmpi(key, manifest[mid], kl);
#ifdef LOOKUP_DEBUG
        printf("`%s` vs `%s`: %d(%d)\n", key, manifest[mid], mid, search);
#endif
        /**/ if( search < 0 ) max = mid - 1;
        else if( search > 0 ) min = mid + 1;
        else { found = mid; break; }
    }

    return found >= 0 ? strreverse(va("%s", manifest[found])) : strreverse(key);
}
