extern const char *(*flag )(const char *opt); // --arg, --arg=hello
extern const char *(*flags)(const char *opt, const char *defaults); // --arg=hello
extern int         (*flagi)(const char *opt, int defaults); // --arg=10
extern float       (*flagf)(const char *opt, float defaults); // --arg=3.14

#ifdef CODE
#pragma once
#include v3_flag_backend

int (*flagi)(const char *opt, int) = flagi_;
float (*flagf)(const char *opt, float) = flagf_;
const char *(*flag)(const char *opt) = flag_;
const char *(*flags)(const char *opt, const char *) = flags_;
#endif
