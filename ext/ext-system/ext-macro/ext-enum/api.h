#ifndef ENUM_H
#define ENUM_H

#define enum(name) int name; enum

#define TYPEDEF_ENUM(F,...) \
typedef enum(F) { __VA_ARGS__ }; static const char *F##_STRING = #__VA_ARGS__; \
static inline int F##int(const char *s) { return ENUMint(s, F##_STRING); } \
static inline const char* F##str(int e) { return ENUMstr(e, F##_STRING); }

extern int (*ENUMint)(const char *e, const char *ENUM_STRING);
extern const char* (*ENUMstr)(int e, const char *ENUM_STRING);

#elif CODE
#pragma once

int ENUMint__(const char *e, const char *ENUM_STRING) {
    int rc = -1;
    for each_string(s, ENUM_STRING, ",") {
        const char *eq = strchr(s, '=');
        if(eq) rc = atoi(eq+1); else ++rc;
        if( strstr(s,e) ) return rc;
    }
    return -1;
}
int ENUMint_(const char *exp, const char *ENUM_STRING) {
    unsigned f = 0;
    for each_string(flag, exp, "+|;,") {
        for each_string(token, flag, "\t\r\n ") {
            int m = ENUMint__(token, ENUM_STRING);
            if( m >= 0 ) f |= m;
            break;
        }
    }
    return f;
}
const char* ENUMstr_(int e, const char *ENUM_STRING) {
    int rc = -1;
    for each_string(s, ENUM_STRING, ",") {
        const char *eq = strchr(s, '=');
        if(eq) rc = atoi(eq+1); else ++rc;
        if(rc == e) for each_string(z, s, " =") return va("%s", z);
    }
    return "";
}

int (*ENUMint)(const char *e, const char *ENUM_STRING) = ENUMint_;
const char* (*ENUMstr)(int e, const char *ENUM_STRING) = ENUMstr_;

#endif
