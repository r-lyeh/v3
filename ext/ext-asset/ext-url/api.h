// downloads url. returns associated local file. or null if could not download.
extern const char *(*url)(const char *u);

// same than above, but does file caching. if fstamp > expired date, downloads again.
extern const char *(*urlcache)(const char *u, uint64_t expired);

#if CODE
#pragma once
#include v3_url_backend

const char *(*url)(const char *u) = url_;
const char *(*urlcache)(const char *u, uint64_t expired) = urlcache_;
#endif
