extern THREAD const char *file_;
extern THREAD int line_;

#ifndef FILELINE
#define FILELINE(...) (file_ = __FILE__, line_ = __LINE__, __VA_ARGS__)
#endif

#if CODE
#pragma once

THREAD const char *file_ = "";
THREAD int line_ = 0;

#endif

// #define myfunc(...) FILELINE(myfunc(__VA_ARGS__))
// int (myfunc)(const char *s) {
//     return puts(va("%s [%d]", s, file_, line_));
// }
