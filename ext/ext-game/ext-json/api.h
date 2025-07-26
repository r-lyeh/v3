// ----------------------------------------------------------------------------
// json api

extern bool    (*json_push)(const char *source);
extern bool    (*json_pushf)(const char *pathfile);
extern int      (*json_int)(const char *pathkey);
extern double   (*json_float)(const char *pathkey);
extern char*    (*json_string)(const char *pathkey);
extern int      (*json_count)(const char *pathkey);
extern void*    (*json_node)(const char *pathkey);
extern bool    (*json_pop)(void);

#if CODE
#pragma once
#include v3_json_backend

bool  (*json_push)(const char *) = json_push_;
bool  (*json_pushf)(const char *) = json_pushf_;
int    (*json_int)(const char *) = json_int_;
double (*json_float)(const char *) = json_float_;
char*  (*json_string)(const char *) = json_string_;
int    (*json_count)(const char *) = json_count_;
void*  (*json_node)(const char *) = json_node_;
bool  (*json_pop)(void) = json_pop_;
#endif
