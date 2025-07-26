#ifndef FILE_H
#define FILE_H

typedef struct file_t {
    void *bin; unsigned len;
    char url[64];
} file_t;

extern file_t     (*blob)(void *bin, unsigned len);

extern file_t     (*file)(const char *pathfile);      // read file, applying processing hooks
extern file_t     (*file_read)(const char *pathfile); // read file. no processing
extern unsigned   (*file_size)(const char *pathfile); // sizeof file
extern bool       (*file_save)(const char *pathfile, const void *bin, unsigned len); // overwrite file

extern char*      (*path     )(const char *pathfile); // /path/to/file.txt > /path/to/
extern char*      (*path_name)(const char *pathfile); // /path/to/file.txt > file.txt
extern char*      (*path_base)(const char *pathfile); // /path/to/file.txt > file
extern char*      (*path_ext )(const char *pathfile); // /path/to/file.txt > .txt
extern char*      (*path_find)(const char *pathfile); // file.txt > ./path/to/file.txt
extern char*      (*path_unix)(const char *pathfile); // convert slashes+quotes to unix
extern char*      (*path_norm)(const char *pathfile); // convert slashes+quotes to native
extern int        (*path_stat)(const char *pathfile); // 0 not found, 1 file, 2 dir

#elif CODE
#pragma once

#ifdef _WIN32
#define open8(path,mode)  _wopen(widen(path))
#define popen8(path,mode) _wpopen(widen(path),widen(mode))
#define fopen8(path,mode) _wfopen(widen(path),widen(mode))
#define remove8(path)     _wremove(widen(path))
#define rename8(path)     _wrename(widen(path))
#define stat8(path,st)    _wstat(widen(path),st) // _stati64()
#define stat8_t           _stat                  // struct _stati64
#else
#define open8(path,mode)  open(path,mode)
#define popen8(path,mode) popen(path,mode)
#define fopen8(path,mode) fopen(path,mode)
#define remove8(path)     remove(path)
#define rename8(path)     rename(path)
#define stat8(path,st)    stat(path,st)    // _stati64()
#define stat8_t           stat             // struct _stati64
#endif

#include v3_file_backend
#include v3_path_backend
#include "path_find.h"

#if 1
typedef void*  (*filename_hookfn_t)(void*);
typedef file_t (*postread_hookfn_t)(file_t);

filename_hookfn_t prev[8]; unsigned prev_idx; // prev read
postread_hookfn_t post[8]; unsigned post_idx; // post read

void* file_hook_filename(filename_hookfn_t fn) {
    assert( prev_idx < COUNTOF(prev) );
    return prev[prev_idx++] = fn;
}
void* file_hook_postread(postread_hookfn_t fn) {
    assert( post_idx < COUNTOF(post) );
    return post[prev_idx++] = fn;
}
#endif

file_t file_(const char *partial_pathfile) {
    char *pathfile = path_find(partial_pathfile);

    // preprocess filename (redirects, etc)
    for( int pass = 0; prev[pass]; ) {
        char *name2 = prev[pass](pathfile);
        if( !name2 ) pass = 0, pathfile = name2;
        else ++pass;
    }

    // cook asset
    pathfile = (char*)cook(pathfile);

    // io read
    file_t dst = file_read(pathfile);

    // postprocess contents
    for( int pass = 0; post[pass]; ++pass ) {
        dst = post[pass](dst);
    }

    return dst;
}

file_t      (*blob)(void *, unsigned) = blob_;

file_t      (*file)(const char *) = file_;
file_t      (*file_read)(const char *) = file_read_;
unsigned    (*file_size)(const char *) = file_size_;
bool        (*file_save)(const char *, const void *, unsigned) = file_save_;

char*       (*path     )(const char *) = path_;
char*       (*path_name)(const char *) = path_name_;
char*       (*path_base)(const char *) = path_base_;
char*       (*path_ext )(const char *) = path_ext_;
char*       (*path_find)(const char *) = path_find_;
char*       (*path_unix)(const char *) = path_unix_;
char*       (*path_norm)(const char *) = path_norm_;
int         (*path_stat)(const char *) = path_stat_;
#endif
