char *replace(char *text, const char *target, const char *replacement); // replace all occurences of substring in text only if replacement is shorter or equal than original string

#if CODE
#pragma once

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
