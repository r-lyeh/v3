#ifndef FILE_H
#define FILE_H

typedef struct file_t {
    union {
        void *bin;
        char *str;
    };
    unsigned len;
    char url[64];
} file_t;

extern file_t     (*file)(const char *pathfile);      // locate and read file. no processing
extern file_t     (*file_read)(const char *pathfile); // read file, no processing.
extern file_t     (*file_cook)(const char *pathfile); // read file, applying processing hooks
extern unsigned   (*file_size)(const char *pathfile); // sizeof file
extern bool       (*file_save)(const char *pathfile, const void *bin, unsigned len); // overwrite file
extern bool       (*file_join)(const char *pathfile, const void *bin, unsigned len); // append to file
extern uint64_t   (*file_date)(const char *pathfile);

#elif CODE
#pragma once

#ifdef _WIN32
#define open8(path,mode)  _wopen(strwide(path))
#define popen8(path,mode) _wpopen(strwide(path),strwide(mode))
#define fopen8(path,mode) _wfopen(strwide(path),strwide(mode))
#else
#define open8(path,mode)  open(path,mode)
#define popen8(path,mode) popen(path,mode)
#define fopen8(path,mode) fopen(path,mode)
#endif

#include v3_file_backend

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
    char *pathfile = lookup(partial_pathfile);
    return file_read_(pathfile);
}

file_t file_cook_(const char *partial_pathfile) {
    char *pathfile = lookup(partial_pathfile);

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

file_t      (*file)(const char *) = file_;
file_t      (*file_read)(const char *) = file_read_;
file_t      (*file_cook)(const char *) = file_cook_;
unsigned    (*file_size)(const char *) = file_size_;
bool        (*file_save)(const char *, const void *, unsigned) = file_save_;
bool        (*file_join)(const char *, const void *, unsigned) = file_join_;
uint64_t    (*file_date)(const char *) = file_date_;

#endif
