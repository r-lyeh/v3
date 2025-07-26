char *path_name_(const char *pathfile) { // /path/to/file.txt > file.txt
    const char *a = strrchr(pathfile, '/');
    const char *b = strrchr(pathfile, '\\');
    return va("%s", a > b ? a+1 : b > a ? b+1 : pathfile);
}
char *path_(const char *pathfile) { // /path/to/file.txt > /path/to/
    const char *a = strrchr(pathfile, '/');
    const char *b = strrchr(pathfile, '\\');
    return va("%.*s", (int)(strlen(pathfile) - strlen(path_name_(pathfile))), pathfile);
}
char *path_ext_(const char *pathfile) { // /path/to/file.txt > .txt
    char *name = path_name_(pathfile), *dot = strchr(name, '.');
    return dot ? dot : name + strlen(name);
}
char *path_base_(const char *pathfile) { // /path/to/file.txt > file
    char *name = path_name_(pathfile), *dot = strchr(name, '.');
    return dot ? (*dot = '\0', name) : name;
}
char *path_unix_(const char *pathfile) { // /path/to/file.txt
    char *copy = va("%s", 2 * !!(pathfile[0] && pathfile[1] == ':') + pathfile);
    return replace(copy, "\\", "/");
}
char *path_norm_(const char *pathfile) { // /path/to/file.txt or \path\to\file.txt
    char *p = ifdef(win32, replace(path_unix_(pathfile), "/", "\\"), path_unix_(pathfile));
    return ifdef(win32, replace(p, "'", "\""), replace(p, "\"", "'"));
}
int path_stat_( const char *pathfile ) { // 0 not found, 1 file, 2 dir
    ifdef(tcc, ifndef(win32,, // trim ending slashes on tcc+win32 case
        char *p = va("%s", pathfile), *q = *p ? p+strlen(p)-1 : p;
        while( strchr("\\/", *q) ) *q-- = 0; pathfile = p;
    ))
    struct stat8_t st;
    return stat8(pathfile, &st) >= 0 ? 1 + (S_IFDIR == ( st.st_mode & S_IFMT )) : 0;
#if 0 // !__has_include(sys/stat)
    for( FILE *fp = fopen8(pathfile, "rb"); fp; ) return fclose(fp), 1;
    for( FILE *fp = fopen8(pathfile, "w+b"); fp; ) return fclose(fp), unlink(pathfile), 0;
    return 2;
#endif
}
