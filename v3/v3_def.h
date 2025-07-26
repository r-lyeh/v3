// some required defines before any other header is included
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

// common headers to all platforms
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ----------------------------------------------------------------------------
// visibility
// win32 users may need to use -DAPI=EXPORT/IMPORT/STATIC as needed when using DLLs.

#ifndef IMPORT
#define IMPORT ifdef(win32, ifdef(gcc, __attribute__ ((dllimport)), __declspec(dllimport)))
#endif
#ifndef EXPORT
#define EXPORT ifdef(win32, ifdef(gcc, __attribute__ ((dllexport)), __declspec(dllexport)))
#endif
#ifndef STATIC
#define STATIC
#endif
#ifndef API
#define API    STATIC //EXPORT // public visibility by default
#endif

// ----------------------------------------------------------------------------
// if/n/def hell

#define ifdef(macro, yes, /*no*/...)   ifdef_##macro(yes, __VA_ARGS__)
#define ifndef(macro, yes, /*no*/...)  ifdef_##macro(__VA_ARGS__, yes)
#define ifdef_1(yes, /*no*/...)        yes
#define ifdef_0(yes, /*no*/...)        __VA_ARGS__

#ifdef __cplusplus
#define ifdef_c                        ifdef_0
#define ifdef_cpp                      ifdef_1
#else
#define ifdef_c                        ifdef_1
#define ifdef_cpp                      ifdef_0
#endif

#ifdef _MSC_VER
#define ifdef_gcc                      ifdef_0
#define ifdef_mingw                    ifdef_0
#define ifdef_tcc                      ifdef_0
#define ifdef_cl                       ifdef_1
#elif defined __TINYC__
#define ifdef_gcc                      ifdef_0
#define ifdef_mingw                    ifdef_0
#define ifdef_tcc                      ifdef_1
#define ifdef_cl                       ifdef_0
#elif defined __MINGW64__ || defined __MINGW32__
#define ifdef_gcc                      ifdef_1
#define ifdef_mingw                    ifdef_1
#define ifdef_tcc                      ifdef_0
#define ifdef_cl                       ifdef_0
#else // also __clang__
#define ifdef_gcc                      ifdef_1
#define ifdef_mingw                    ifdef_0
#define ifdef_tcc                      ifdef_0
#define ifdef_cl                       ifdef_0
#endif

#ifndef __clang__
#  define ifdef_clang   ifdef_0
#  define ifdef_clangcl ifdef_0
#else
#  define ifdef_clang   ifdef_1
#  ifdef _MSC_VER
#  define ifdef_clangcl ifdef_1
#  else
#  define ifdef_clangcl ifdef_0
#  endif
#endif

#if defined _WIN32
#define ifdef_win32                    ifdef_1
#define ifdef_linux                    ifdef_0
#define ifdef_osx                      ifdef_0
#define ifdef_bsd                      ifdef_0
#define ifdef_ems                      ifdef_0
#elif defined __linux__ || defined __linux
#define ifdef_win32                    ifdef_0
#define ifdef_linux                    ifdef_1
#define ifdef_osx                      ifdef_0
#define ifdef_bsd                      ifdef_0
#define ifdef_ems                      ifdef_0
#elif defined __APPLE__
#define ifdef_win32                    ifdef_0
#define ifdef_linux                    ifdef_0
#define ifdef_osx                      ifdef_1
#define ifdef_bsd                      ifdef_0
#define ifdef_ems                      ifdef_0
#elif defined __EMSCRIPTEN__
#define ifdef_win32                    ifdef_0
#define ifdef_linux                    ifdef_0
#define ifdef_osx                      ifdef_0
#define ifdef_bsd                      ifdef_0
#define ifdef_ems                      ifdef_1
#else // __FreeBSD__ || @todo: __ANDROID_API__
#define ifdef_win32                    ifdef_0
#define ifdef_linux                    ifdef_0
#define ifdef_osx                      ifdef_0
#define ifdef_bsd                      ifdef_1
#define ifdef_ems                      ifdef_0
#endif

// rely on NDEBUG as the official/portable way to disable asserts
// we extend NDEBUG=[0,1,2,3] to signal the compiler optimization flags O0,O1,O2,O3
#if   defined NDEBUG && NDEBUG >= 3
#define ifdef_retail                   ifdef_1
#define ifdef_release                  ifdef_0
#define ifdef_debug                    ifdef_0
#elif defined NDEBUG && NDEBUG >= 2
#define ifdef_retail                   ifdef_0
#define ifdef_release                  ifdef_1
#define ifdef_debug                    ifdef_0
#elif defined NDEBUG && NDEBUG >= 1
#define ifdef_retail                   ifdef_0
#define ifdef_release                  ifdef_1
#define ifdef_debug                    ifdef_0
#else
#define ifdef_retail                   ifdef_0
#define ifdef_release                  ifdef_0
#define ifdef_debug                    ifdef_1
#endif

#if (defined INTPTR_MAX && INTPTR_MAX == INT64_MAX) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__) || defined(__x86_64) || defined(__ppc64__) || __WORDSIZE == 64
#define ifdef_x64                      ifdef_1
#define ifdef_x32                      ifdef_0
#else
#define ifdef_x64                      ifdef_0
#define ifdef_x32                      ifdef_1
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define ifdef_big                      ifdef_1
#define ifdef_little                   ifdef_0
#else
#define ifdef_little                   ifdef_1
#define ifdef_big                      ifdef_0
#endif

#define ifdef_true  ifdef_1
#define ifdef_false ifdef_0

// ----------------------------------------------------------------------------
// extra macros

#ifndef ASSERT // extended assert() that works in retail builds
#define ASSERT(x) for( int _ = !!(x), __ = _ ? 1 : (PERROR(#x), exit(-__LINE__), 0) ; _ ; _ = 0 )
#endif

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

#ifndef COUNTOF // count items in an array at compile time
#define COUNTOF(x) ((int)(sizeof(x)/sizeof(0[x])))
#endif

#ifndef DEFINED
#define DEFINED(x) ifdef(x,1,0)
#endif

#ifndef JOIN // joins two tokens at compile time
#define JOIN(x,y) J0IN(x,y)
#define J0IN(x,y) x##y
#endif

#ifndef MACRO // create unique identifier per line basis
#define MACRO(name) JOIN(name, __LINE__)
#endif

#ifndef ONCE // execute next scope only once
#define ONCE static int MACRO(once) = 1; for(;MACRO(once);MACRO(once)=0)
#endif

#ifndef PERROR // extended perror() that prints file:line
#define PERROR(x) perror(JOIN(__FILE__":",STRINGIZE(__LINE__)) " " x)
#endif

#ifndef STATIC_ASSERT // assert at compile time
#define STATIC_ASSERT(x) typedef int statics[(x)?1:-1]
#endif

#ifndef STRINGIZE // convert a token into a string at compile time
#define STRINGIZE(x) S7RINGIZE(x)
#define S7RINGIZE(x) #x
#endif

#ifndef UNIQUE // create unique identifier
#define UNIQUE(name) JOIN(JOIN(JOIN(name,JOIN(_L,__LINE__)),_),__COUNTER__)
#endif

// --- glue code

#ifndef REALLOC // common entry point for all our memory ops
#define REALLOC realloc
#endif

#ifndef THREAD // thread local storage
#define THREAD ifdef(cl, __declspec(thread), __thread)
#endif

#ifndef POPEN
#define POPEN  ifdef(cl, _popen, popen)
#endif

#ifndef PCLOSE
#define PCLOSE ifdef(cl, _pclose, pclose)
#endif

#ifndef ALLOCA
#define ALLOCA ifdef(cl, _alloca, alloca)
#endif

#ifndef FLOCKFILE
#define FLOCKFILE ifdef(cl,_lock_file,ifdef(mingw,_lock_file,flockfile))
#endif

#ifndef FUNLOCKFILE
#define FUNLOCKFILE ifdef(cl,_unlock_file,ifdef(mingw,_unlock_file,funlockfile))
#endif

#ifndef ATOI64
#define ATOI64 ifdef(cl, _atoi64, atoll/*atoi64*/)
#endif

#if 0

#ifndef CHDIR
#define CHDIR  ifdef(cl, _chdir, chdir)
#endif

#ifndef MKDIR // @todo: win32 function is recursive, probably the others should be as well
#ifdef _WIN32
#define MKDIR(p) mkdir(p)
#else
#define MKDIR(p) mkdir(p,0777)
#endif
#endif

#ifndef FTRUNCATE
#define FTRUNCATE ifdef(cl, _chsize_s, ftruncate)
#endif
#endif
    