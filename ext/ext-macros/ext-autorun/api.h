#ifndef AUTORUN_H
#define AUTORUN_H

#ifndef AUTORUN // execute next global scope before main() is reached
#define AUTORUN AUTORUN_( UNIQUE(fn) )
#ifdef __cplusplus
#define AUTORUN_(fn) \
    static void fn(void); \
    static const int JOIN(fn,__1) = (fn(), 1); \
    static void fn(void)
#elif defined _MSC_VER && !defined(__clang__) // cl, but not clang-cl
#define AUTORUN_(fn) \
    static void fn(void); \
    static int JOIN(fn,__1) (){ fn(); return 0; } \
    __pragma(section(".CRT$XIU", long, read)) \
    __declspec(allocate(".CRT$XIU")) \
    static int(* JOIN(fn,__2) )() = JOIN(fn,__1); \
    static void fn(void)
#elif defined __TINYC__ // tcc...
#define AUTORUN_(fn) \
    __attribute((constructor)) \
    static void fn(void)
#else // gcc,clang,clang-cl...
#define AUTORUN_(fn) \
    __attribute__((constructor(__COUNTER__+101))) \
    static void fn(void)
#endif
// note: based on code by Joe Lowe (public domain).
// note: XIU for C initializers, XCU for C++ initializers, XTU for C deinitializers
#endif

#endif
