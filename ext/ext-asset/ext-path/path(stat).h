#if V3_CL
#include <sys/types.h>
#include <sys/stat.h>
#endif

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
    char *p = ifdef(V3_WINDOWS, replace(path_unix_(pathfile), "/", "\\"), path_unix_(pathfile));
    return ifdef(V3_WINDOWS, replace(p, "'", "\""), replace(p, "\"", "'"));
}
unsigned path_stat_( const char *pathfile ) { // 0 not found, 1 file, 2 dir
    ifdef(V3_TCC, ifdef(V3_WINDOWS, { // trim ending slashes on tcc+win32 case
        char *p = va("%s", pathfile), *q = *p ? p+strlen(p)-1 : p;
        while( strchr("\\/", *q) ) *q-- = 0; pathfile = p;
    }))
    struct stat8_t st;
    return stat8(pathfile, &st) >= 0 ? 1 + (S_IFDIR == ( st.st_mode & S_IFMT )) : 0;
#if 0 // !__has_include(sys/stat)
    for( FILE *fp = fopen8(pathfile, "rb"); fp; ) return fclose(fp), 1;
    for( FILE *fp = fopen8(pathfile, "w+b"); fp; ) return fclose(fp), unlink(pathfile), 0;
    return 2;
#endif
}
unsigned path_size_(const char *pathfile) {
    struct stat8_t st;
    return stat8(pathfile, &st) >= 0 ? (unsigned)st.st_size : 0u;
}
uint64_t path_date_(const char *pathfile) {
    struct stat8_t st;
    return stat8(pathfile, &st) >= 0 ? (uint64_t)st.st_mtime : 0ULL;
}
/*
uint64_t path_date10_(const char *fname) {
    time_t mtime = (time_t)path_date_(fname);
    struct tm *ti = localtime(&mtime);
    return atoi64_(va("%04d%02d%02d%02d%02d%02d",ti->tm_year+1900,ti->tm_mon+1,ti->tm_mday,ti->tm_hour,ti->tm_min,ti->tm_sec));
}
*/
char* path_find_(const char *pathfile) {
    return lookup(pathfile);
}
