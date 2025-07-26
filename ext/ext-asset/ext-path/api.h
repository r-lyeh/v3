#ifndef PATH_H
#define PATH_H

extern char*      (*path     )(const char *pathfile); // /path/to/file.txt > /path/to/
extern char*      (*path_name)(const char *pathfile); // /path/to/file.txt > file.txt
extern char*      (*path_base)(const char *pathfile); // /path/to/file.txt > file
extern char*      (*path_ext )(const char *pathfile); // /path/to/file.txt > .txt
extern char*      (*path_find)(const char *pathfile); // file.txt > ./path/to/file.txt
extern char*      (*path_unix)(const char *pathfile); // convert slashes+quotes to unix
extern char*      (*path_norm)(const char *pathfile); // convert slashes+quotes to native
extern unsigned   (*path_stat)(const char *pathfile); // 0 not found, 1 file, 2 dir    @todo: 3 symlink, 4...
extern unsigned   (*path_size)(const char *pathfile);
extern uint64_t   (*path_date)(const char *pathfile);

#elif CODE
#pragma once

#ifdef _WIN32
#define remove8(path)     _wremove(strwide(path))
#define rename8(path)     _wrename(strwide(path))
#define stat8(path,st)    _wstat(strwide(path),st) // _stati64()
#define stat8_t           _stat                  // struct _stati64
#else
#define remove8(path)     remove(path)
#define rename8(path)     rename(path)
#define stat8(path,st)    stat(path,st)    // _stati64()
#define stat8_t           stat             // struct _stati64
#endif

#include v3_path_backend

char*       (*path     )(const char *) = path_;
char*       (*path_name)(const char *) = path_name_;
char*       (*path_base)(const char *) = path_base_;
char*       (*path_ext )(const char *) = path_ext_;
char*       (*path_find)(const char *) = path_find_;
char*       (*path_unix)(const char *) = path_unix_;
char*       (*path_norm)(const char *) = path_norm_;
unsigned    (*path_stat)(const char *) = path_stat_;
unsigned    (*path_size)(const char *) = path_size_;
uint64_t    (*path_date)(const char *) = path_date_;
#endif
