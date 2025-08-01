#define STB_SPRINTF_IMPLEMENTATION
#include "3rd/stb_sprintf.h"

#if defined __GNUC__ || defined __clang__
__attribute__((no_sanitize_address))
#elif defined _MSC_VER && _MSC_VER > 1900
__declspec(no_sanitize_address)
#endif
char* tempvl(const char *fmt, va_list vl) {
    va_list copy;
    va_copy(copy, vl);
    int needed = stbsp_vsnprintf( 0, 0, fmt, copy ) + 1; assert(needed >= 1);
    va_end(copy);

    static THREAD int STACK_ALLOC = 256*1024;
    static THREAD char *buf = 0; if(!buf) buf = (char*)REALLOC(0, STACK_ALLOC);
    static THREAD int cur = 0;

    char* ptr = buf + (cur *= (cur+needed) < (STACK_ALLOC - 1), (cur += needed) - needed); assert(ptr);
    int rc = (*ptr = 0, stbsp_vsnprintf( ptr, needed, fmt, vl )); assert(rc >= 0);
    return (char *)ptr;
}
char* va_(const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    char *s = tempvl(fmt, vl);
    va_end(vl);
    return s;
}
