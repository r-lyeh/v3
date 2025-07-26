#ifndef STRINGIZE // convert a token into a string at compile time
#define STRINGIZE(x) S7RINGIZE(x)
#define S7RINGIZE(x) #x
#endif
