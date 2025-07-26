int strcmpq(const void *a, const void *b);         // qsort-style strcmp
int strcmpqi(const void *a, const void *b);        // qsort-style strcmp. case insensitive
int strncmpi(const char *a, const char *b, int n); // strncmp. case insensitive

#if CODE
#pragma once

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
    return strcmpi_(*ia, *ib);
}
// strncmp. case insensitive
int strncmpi(const char *a, const char *b, int n) {
    while( n && *a && tolower(*a) == tolower(*b) ) {
        --n, ++a, ++b;
    }
    return n ? tolower(*a) - tolower(*b) : 0;
}

#endif
