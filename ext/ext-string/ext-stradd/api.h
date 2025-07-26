char* stradd(char **dst_, const char *buf); // concat and resize input string if needed. free() after use. @todo: use drop() instead

#if CODE
#pragma once

char* stradd(char **dst_, const char *buf) { // free() after use. @todo: use drop() instead
    char *dst = *dst_;
        if(!buf) return dst;
        // if(!buf[0]) return dst;
        int slen = (dst ? strlen(dst) : 0), buflen = strlen(buf);
        dst = (char*)realloc(dst, slen + buflen + 1); // beware. do not use resize() yet
        memcpy(dst + slen, buf, buflen + 1);
    *dst_ = dst;
    return dst;
}

#endif
