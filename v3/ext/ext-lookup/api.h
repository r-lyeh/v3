#ifndef LOOKUP_H
#define LOOKUP_H

extern char* (*lookup)(const char *pathfile); // file.txt > ./path/to/file.txt

#elif CODE
#pragma once
#include v3_lookup_backend

char* (*lookup)(const char *) = lookup_;
#endif
