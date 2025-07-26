extern THREAD const char * const FILE_;
extern THREAD int LINE_;

#ifndef FILELINE
#define FILELINE(...) (FILE_ = __FILE__, LINE_ = __LINE__, __VA_ARGS__)
#endif

#if CODE
#pragma once

THREAD const char * const FILE_ = "";
THREAD int LINE_ = 0;

#endif

// int myfunc(const char *s) {
//     return puts(va("%s [%d]", s, FILE_, LINE_));
// }
// #define myfunc(...) FILELINE(myfunc(__VA_ARGS__))
