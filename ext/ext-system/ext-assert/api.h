#ifndef ASSERT // extended assert() that works in retail builds
#define ASSERT(x) for( int _ = !!(x), __ = _ ? 1 : (perror(JOIN(__FILE__":",STRINGIZE(__LINE__)) " " #x), exit(-__LINE__), 0) ; _ ; _ = 0 )
#endif

#ifndef STATIC_ASSERT // assert at compile time
#define STATIC_ASSERT(x) typedef int statics[(x)?1:-1]
#endif
