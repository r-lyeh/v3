extern char* (*lookup)(const char *pathfile);        // file.txt > ./path/to/file.txt

#if CODE
#pragma once
#include v3_lookup_backend

char*         (*lookup)(const char *) = lookup_;
#endif
