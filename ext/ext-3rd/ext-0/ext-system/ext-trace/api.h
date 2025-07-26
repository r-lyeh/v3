// @todo: trace(int num, const char *fmt = "%03i %a %n (%l)\n"); // [i]ndex [a]ddress [n]ame [l]ocation

extern char *(*trace)(int num); // generate N backtraces. flip sign to reverse order. do not free()
extern char *(*tracef)(FILE *fp, int num); // generate N backtraces and write them. flip sign to reverse order. do not free()

#if CODE
#pragma once
#include v3_trace_backend

char *(*trace)(int num) = trace_;
char *(*tracef)(FILE *fp, int num) = tracef_;
#endif
