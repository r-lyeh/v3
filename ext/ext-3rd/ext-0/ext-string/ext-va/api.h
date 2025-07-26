extern char* (*va)(const char *fmt, ...); // construct temporary string() that gets wiped over time. no need to free()

#if CODE
#pragma once
#include v3_va_backend

char* (*va)(const char *fmt, ...) = va_;

#endif
