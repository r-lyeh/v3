extern char *(*trace)(int num); // generate N backtraces. flip sign to reverse order.
extern char *(*tracef)(FILE *fp, int num); // generate N backtraces and write them. flip sign to reverse order.

#ifdef CODE
#pragma once
#include v3_trace_backend

char *(*trace)(int num) = trace_;
char *(*tracef)(FILE *fp, int num) = tracef_;

#endif
