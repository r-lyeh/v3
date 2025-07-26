#ifndef MACRO // create unique identifier per line basis
#define MACRO(name) JOIN(name, __LINE__)
#endif
