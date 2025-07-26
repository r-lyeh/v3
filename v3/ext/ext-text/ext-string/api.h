int strmatch(const char *text, const char *wildcard);  // returns true if wildcard matches
int strmatchi(const char *text, const char *wildcard); // returns true if wildcard matches (case insensitive)

const char *strstri(const char *text, const char *sub); // finds substring in text. case insensitive
const char *strbegi(const char *text, const char *sub); // finds substring at beginning of text. case insensitive
const char *strendi(const char *text, const char *sub); // finds substring at end of text. case insensitive

const char *strrstr(const char *text, const char *sub); // last occurence of substring in text. similar to strrchr()
const char *strrstri(const char *text,const char *sub); // last occurence of substring in text. similar to strrchr(). case insensitive

unsigned    strcnt(const char *text, char ch); // counts occurences of character in text

char *      replace(char *text, const char *target, const char *replacement); // replace all occurences of substring in text only if replacement is shorter or equal than original string

int strcmpq(const void *a, const void *b);         // qsort-style strcmp
int strcmpqi(const void *a, const void *b);        // qsort-style strcmp. case insensitive
int strncmpi(const char *a, const char *b, int n); // strncmp. case insensitive

char* stradd(char **dst_, const char *buf); // concat and resize input string if needed. free() after use. @todo: use drop() instead

const char* strvalid(const char *text); // prevents passing null string

#if CODE
#pragma once

int strmatch(const char *s, const char *wildcard) {
    if( *wildcard == '\0' ) return !*s;
    if( *wildcard ==  '*' ) return strmatch(s, wildcard+1) || (*s && strmatch(s+1, wildcard));
    if( *wildcard ==  '?' ) return *s && (*s != '.') && strmatch(s+1, wildcard+1);
    return (*s == *wildcard) && strmatch(s+1, wildcard+1);
}
int strmatchi(const char *s, const char *wildcard) {
    if( *wildcard == '\0' ) return !*s;
    if( *wildcard ==  '*' ) return strmatchi(s, wildcard+1) || (*s && strmatchi(s+1, wildcard));
    if( *wildcard ==  '?' ) return *s && (*s != '.') && strmatchi(s+1, wildcard+1);
    return (tolower(*s) == tolower(*wildcard)) && strmatchi(s+1, wildcard+1);
}

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
    return !STRCMPI(src + srclen - sublen, sub) ? src + srclen - sublen : NULL;
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

// strcmp. qsort-style
int strcmpq(const void *a, const void *b) {
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}
// strcmp. qsort-style. case insensitive
int strcmpqi(const void *a, const void *b) {
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmpi(*ia, *ib);
}
// strncmp. case insensitive
int strncmpi(const char *a, const char *b, int n) {
    while( n && *a && tolower(*a) == tolower(*b) ) {
        --n, ++a, ++b;
    }
    return n ? tolower(*a) - tolower(*b) : 0;
}

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

const char* strvalid(const char *text) {
    return text ? text : "";
}

#endif
