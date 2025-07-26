#ifndef COUNTOF // count items in an array at compile time
#define COUNTOF(x) ((int)(sizeof(x)/sizeof(0[x])))
#endif
