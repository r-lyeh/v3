int strmatch(const char *text, const char *wildcard);  // returns true if wildcard matches
int strmatchi(const char *text, const char *wildcard); // returns true if wildcard matches (case insensitive)

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

#endif
