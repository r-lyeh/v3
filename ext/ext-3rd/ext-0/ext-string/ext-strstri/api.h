const char *strstri(const char *text, const char *sub); // finds substring in text. case insensitive
const char *strbegi(const char *text, const char *sub); // finds substring at beginning of text. case insensitive
const char *strendi(const char *text, const char *sub); // finds substring at end of text. case insensitive

const char *strrstr(const char *text, const char *sub); // last occurence of substring in text. similar to strrchr()
const char *strrstri(const char *text,const char *sub); // last occurence of substring in text. similar to strrchr(). case insensitive

#if CODE
#pragma once

const char *strstri(const char *a, const char *b) {
    char *A, *B, *M;
    for(char *p = (char*)(A = va("%s",a)); *p; ++p) *p = toupper(*p);
    for(char *p = (char*)(B = va("%s",b)); *p; ++p) *p = toupper(*p);
    return M = strstr(A, B), M ? a + (M - A) : NULL;
}
const char *strbegi(const char *a, const char *b) {
    const char *A = strstri(a, b);
    return A == a ? a : NULL;
}
const char *strendi(const char *src, const char *sub) { // returns true if both strings match at end. case insensitive
    int srclen = strlen(src);
    int sublen = strlen(sub);
    if( sublen > srclen ) return 0;
    return !strcmpi_(src + srclen - sublen, sub) ? src + srclen - sublen : NULL;
}

const char *strrstr(const char *text, const char *sub) { // last occurence of substring in text. similar to strrchr()
    const char *found = strstr(text, sub);
    while( found ) {
        const char *found2 = strstr(found+1, sub);
        if( !found2 ) return found; else found = found2;
    }
    return found;
}
const char *strrstri(const char *text, const char *sub) { // last occurence of substring in text. similar to strrchr(). case insensitive
    const char *found = strstri(text, sub);
    while( found ) {
        const char *found2 = strstri(found+1, sub);
        if( !found2 ) return found; else found = found2;
    }
    return found;
}

#endif
