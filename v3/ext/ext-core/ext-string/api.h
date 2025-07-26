const char *strstri(const char *text, const char *sub); // finds substring in text. case insensitive
const char *strbegi(const char *text, const char *sub); // finds substring at beginning of text. case insensitive
const char *strendi(const char *text, const char *sub); // finds substring at end of text. case insensitive

unsigned    strcnt(const char *text, char ch); // counts occurences of character in text

char *      replace(char *text, const char *target, const char *replacement); // replace all occurences of substring in text only if replacement is shorter or equal than original string

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
    return !strcmpi(src + srclen - sublen, sub) ? src + srclen - sublen : NULL;
}

unsigned strcnt(const char *text, char ch) {
    unsigned count = 0;
    while(*text++) count += text[-1] == ch;
    return count;
}

char *replace( char *copy, const char *target, const char *replacement ) {
    int rlen = strlen(replacement), diff = strlen(target) - rlen;
    if( diff < 0 ) return 0;
    for( char *s = copy, *e = s + strlen(copy); /*s < e &&*/ 0 != (s = strstr(s, target)); ) {
        if( rlen ) s = (char*)memcpy( s, replacement, rlen ) + rlen;
        if( diff ) memmove( s, s + diff, (e - (s + diff)) + 1 );
    }
    return copy;
}

#endif
