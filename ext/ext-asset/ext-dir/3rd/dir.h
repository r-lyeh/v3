// directory iteration.
// - rlyeh, public domain.

#ifndef DIR_H
#define DIR_H

typedef struct dir dir_t;

dir_t *dir_open(const char *filename, const char *mode); // recursive 'r'

    int dir_find(dir_t*, const char *entryname); // returns entry number; or <0 if not found.
    unsigned dir_count(dir_t*);
        char*    dir_name(dir_t*, unsigned index);
        unsigned dir_size(dir_t*, unsigned index);
        unsigned dir_stat(dir_t*, unsigned index); // 0 not exist, 1 file, 2 dir
        void*    dir_read(dir_t*, unsigned index); // must free() after use

void dir_close(dir_t*);

#endif

// -----------------------------------------------------------------------------

#ifdef DIR_C
//#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#  if defined _WIN32 && defined(__TINYC__)
#include <windows.h>  // tcc
#elif defined _WIN32
#include <winsock2.h> // msc+gcc
#else
#include <dirent.h>
#endif

#ifndef STRDUP
#define STRDUP strdup
#endif

#ifndef STRWIDE
#define STRWIDE strwide
#endif

#ifndef STRSHORT
#define STRSHORT strshort
#endif

#ifndef DIR_REALLOC
#define DIR_REALLOC realloc
#endif

#ifndef DIR_EXCLUDED
#define DIR_EXCLUDED(folder) 0
#endif

#ifndef ERR
#define ERR(NUM, ...) (fprintf(stderr, "" __VA_ARGS__), fprintf(stderr, "(%s:%d)\n", __FILE__, __LINE__), fflush(stderr), (NUM)) // (NUM)
#endif

typedef struct dir_entry {
    char *filename;
    size_t size;
    size_t is_dir : 1;
} dir_entry;

struct dir {
    dir_entry *entry;
    unsigned count;
    bool recursive;
};

// ---

#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#endif

int dir_yield(dir_t *d, const char *pathfile, char *name, int namelen) {
    int ok = 0;
#ifdef _WIN32
    WIN32_FIND_DATAW fdata = { 0 };
    snprintf(name, namelen, "%s/*", pathfile);
    for( HANDLE h = FindFirstFileW(STRWIDE(name), &fdata ); h != INVALID_HANDLE_VALUE; ok = (FindClose( h ), h = INVALID_HANDLE_VALUE, 1)) { // @leak
        for( int next = 1; next; next = FindNextFileW(h, &fdata) != 0 ) {

            int is_dir = (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;

            const char *fname = STRSHORT(fdata.cFileName);
            if( is_dir ) if( DIR_EXCLUDED(fname) ) continue;
            if( is_dir ) if( fname[0] == '.' && (fname[1] == '\0' || fname[1] == '.') ) continue;
            if( is_dir ) if( !strcmp(fname, ".DS_Store") || !strcmp(fname, ".git") || !strcmp(fname, ".github") || !strcmp(fname, ".svn") ) continue;

            snprintf(name, namelen, "%s/%s%s", pathfile, fname, is_dir ? "/" : ""); // @leak

            struct _stat64i32 st; if( !is_dir ) if(_wstat(STRWIDE(name), &st) < 0) continue; //@leak

            // add
            dir_entry de = { STRDUP(name), is_dir ? 0 : st.st_size, is_dir };
            d->entry = (dir_entry*)DIR_REALLOC(d->entry, ++d->count * sizeof(dir_entry));
            d->entry[d->count-1] = de;
            // recurse
            if (is_dir && d->recursive) { char pf[512]; snprintf(pf, 512, "%.*s", (int)strlen(name) - 1, name); name[0] = 0; dir_yield(d, pf, name, namelen); }
        }
    }
#else
    snprintf(name, namelen, "%s/", pathfile);
    for( DIR *dir = opendir(name); dir; ok = (closedir(dir), dir = 0, 1)) {
        for( struct dirent *ep; (ep = readdir(dir)) != NULL; ) {
            snprintf(name, namelen, "%s/%s", pathfile, ep->d_name);
            struct stat st; if( stat(name, &st) < 0 ) continue;
            DIR *tmp = opendir(/*ep->d_*/name); int is_dir = !!tmp; if(tmp) closedir(tmp); // @todo:optimizeme (maybe use stat instead)

            const char *fname = ep->d_name;
            if( is_dir ) if( DIR_EXCLUDED(fname) ) continue;
            if( is_dir ) if( fname[0] == '.' && (fname[1] == '\0' || fname[1] == '.') ) continue;
            if( is_dir ) if( !strcmp(fname, ".DS_Store") || !strcmp(fname, ".git") || !strcmp(fname, ".github") || !strcmp(fname, ".svn") ) continue;

            // add
            if( is_dir ) strcat(name, "/");
            dir_entry de = { STRDUP(name), is_dir ? 0 : st.st_size, is_dir };
            if( is_dir ) name[strlen(name)-1] = '\0';
            d->entry = (dir_entry*)DIR_REALLOC(d->entry, ++d->count * sizeof(dir_entry));
            d->entry[d->count-1] = de;
            // recurse
            if (is_dir && d->recursive) { char pf[512]; snprintf(pf, 512, "%s", name); name[0] = 0; dir_yield(d, pf, name, namelen); }
        }
    }
#endif
    return ok;
}

static
int dir_qsort_(const void *a1, const void *b1) {
    const dir_entry *a = ((const dir_entry*)a1);
    const dir_entry *b = ((const dir_entry*)b1);
#ifdef _MSC_VER
    return strcmpi(a->filename, b->filename);
#else
    return strcasecmp(a->filename, b->filename);
#endif
}

dir_t *dir_open(const char *pathfile, const char *mode) {
    dir_t *d = (dir_t*)DIR_REALLOC(0, sizeof(dir_t)), zero = {0}; *d = zero;
    d->recursive = mode && (mode[0] == 'R' || mode[0] == 'r');

    char *clean = STRDUP( pathfile );
    for( int i = 0; clean[i]; ++i ) if(clean[i] == '\\') clean[i] = '/';
    for( int len = strlen(clean); clean[--len] == '/'; ) clean[len] = '\0';

    char buffer[2048];
    dir_yield(d, clean, buffer, 2048);

#if 1 // ndef _WIN32
    if(d->count) qsort(d->entry, d->count, sizeof(dir_entry), dir_qsort_);
#endif

    clean = DIR_REALLOC(clean, 0);
    return d;
}

int dir_find(dir_t *d, const char *entryname) {
    for( int i = d->count; --i >= 0; ) { // in case of several copies, grab most recent file (last coincidence)
        if( 0 == strcmp(entryname, d->entry[i].filename)) return i;
    }
    return -1;
}

unsigned dir_count(dir_t *d) {
    return d ? d->count : 0;
}

char* dir_name(dir_t *d, unsigned index) {
    return d && index < d->count ? d->entry[index].filename : 0;
}

unsigned dir_size(dir_t *d, unsigned index) {
    return d && index < d->count ? (unsigned)d->entry[index].size : 0;
}

unsigned dir_stat(dir_t *d, unsigned index) {
    return d && index < d->count ? (unsigned)(d->entry[index].is_dir+1) : 0u;
}

void *dir_read(dir_t *d, unsigned index) {
    if( d && index < d->count ) {
        void *data = 0;
        for( FILE *fp = fopen(d->entry[index].filename, "rb"); fp; fclose(fp), fp = 0) {
            size_t len = d->entry[index].size;
            data = DIR_REALLOC(0, len);
            if( data && fread(data, 1, len, fp) != len ) {
                data = DIR_REALLOC(data, 0);
            }
        }
        return data;
    }
    return 0;
}

void dir_close(dir_t *d) {
    for( int i = 0; i < d->count; ++i) {
        d->entry[i].filename = DIR_REALLOC(d->entry[i].filename, 0);
    }
    dir_t zero = {0};
    *d = zero;
    d = DIR_REALLOC(d, 0);
}

#ifdef DIR_DEMO
int main( int argc, char **argv ) {
    dir_t *d = dir_open(argc > 1 ? argv[1] : "./", "rb");
    if( d ) {
        for( int i = 0; i < dir_count(d); ++i ) {
            if( dir_stat(d,i) == 1 )
            printf("%3d) %11d %s\n", i + 1, dir_size(d,i), dir_name(d,i));
            else
            printf("%3d) %11s %s\n", i + 1, "<dir>", dir_name(d,i));
            char *data = dir_read(d,i);
            if(argc > 2 && !strcmp(argv[2],dir_name(d,i))) printf("%.*s\n", dir_size(d,i), data);
            free(data);
        }
        dir_close(d);
    }
}
#define main main__
#endif //DIR_DEMO
#endif //DIR_C
