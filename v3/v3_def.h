// some required defines before any other header is included

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#ifndef OEMRESOURCE
#define OEMRESOURCE
#endif

#ifndef _ALLOW_KEYWORD_MACROS
#define _ALLOW_KEYWORD_MACROS
#endif

// #define _POSIX_C_SOURCE 199309L

// common C headers to all platforms
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef _WIN32
#include <strings.h>
#endif

// ----------------------------------------------------------------------------
// if/n/def hell

#define ifdef( arg, yes, ...)   ifdef2(arg, yes, __VA_ARGS__)
#define ifdef2(arg, yes, ...)   ifdef_##arg(yes, ##__VA_ARGS__)

#define ifndef( arg, yes, ...)  ifndef2(arg, yes, __VA_ARGS__)
#define ifndef2(arg, yes, ...)  ifdef_##arg(__VA_ARGS__, yes)

#define ifdef_1(yes, /*no*/...) yes
#define ifdef_0(yes, /*no*/...) __VA_ARGS__

#ifdef __cplusplus
#define true  1
#define false 0
#endif

// ----------------------------------------------------------------------------

#define V3_ANDROID    0
#define V3_BSD        0
#define V3_LINUX      0
#define V3_MACOS      0
#define V3_WASM       0
#define V3_WINDOWS    0

#define V3_POSIX      0

#define V3_CL         0
#define V3_CLANG      0
#define V3_CLANGCL    0
#define V3_GCC        0
#define V3_MINGW      0
#define V3_TCC        0

#define V3_C          0
#define V3_CPP        0

#define V3_X32        0
#define V3_X64        0

#define V3_BIG        0
#define V3_LITTLE     0

#define V3_DEBUG      0
#define V3_RELEASE    0
#define V3_RETAIL     0


#if defined _WIN32
#undef  V3_WINDOWS
#define V3_WINDOWS    1
#elif defined __linux__ || defined __linux
#undef  V3_LINUX
#define V3_LINUX      1
#elif defined __APPLE__
#undef  V3_MACOS
#define V3_MACOS      1
#elif defined __EMSCRIPTEN__
#undef  V3_WASM
#define V3_WASM       1
#elif defined __ANDROID_API__
#undef  V3_ANDROID
#define V3_ANDROID    1
#endif

#if defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __APPLE__
#undef  V3_BSD
#define V3_BSD        1
#endif

#if !V3_WINDOWS && !V3_ANDROID && !V3_WASM
#undef  V3_POSIX
#define V3_POSIX      1
#endif


#  if defined __TINYC__
#undef  V3_TCC
#define V3_TCC        1
#elif defined __GNUC__
#undef  V3_GCC
#define V3_GCC        1
#elif defined _MSC_VER
#undef  V3_CL
#define V3_CL         1
#endif

#if defined __MINGW64__ || defined __MINGW32__
#undef  V3_MINGW
#define V3_MINGW      1
#endif

#ifdef __clang__
#  undef  V3_CLANG
#  define V3_CLANG    1
#  ifdef _MSC_VER
#  undef  V3_CLANGCL
#  define V3_CLANGCL  1
#  endif
#endif


#ifndef __cplusplus
#undef  V3_C
#define V3_C          1
#else
#undef  V3_CPP
#define V3_CPP        1
#endif


#if (defined INTPTR_MAX && INTPTR_MAX == INT64_MAX) || defined(_M_X64) || defined(__amd64__) || defined(__x86_64__) || defined(__x86_64) || defined(__ppc64__) || __WORDSIZE == 64
#undef  V3_X64
#define V3_X64        1
#else
#undef  V3_X32
#define V3_X32        1
#endif


#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#undef  V3_BIG
#define V3_BIG        1
#else
#undef  V3_LITTLE
#define V3_LITTLE     1
#endif


// rely on NDEBUG as the official/portable way to disable asserts
// we extend NDEBUG=[0,1,2,3] to signal the compiler optimization flags O0,O1,O2,O3
#if   defined NDEBUG && NDEBUG >= 3
#undef  V3_RETAIL
#define V3_RETAIL     1
#elif defined NDEBUG && NDEBUG >= 2
#undef  V3_RELEASE
#define V3_RELEASE    1
#elif defined NDEBUG && NDEBUG >= 1
#undef  V3_RELEASE
#define V3_RELEASE    1
#else
#undef  V3_DEBUG
#define V3_DEBUG      1
#endif

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

#ifndef JOIN // joins two tokens at compile time
#define JOIN(x,y) J0IN(x,y)
#define J0IN(x,y) x##y
#endif

#ifndef MACRO // create unique identifier per line basis
#define MACRO(name) JOIN(name, __LINE__)
#endif

#ifndef ONCE // execute next scope only once
#define ONCE static int MACRO(once) = 1; if(MACRO(once)) if(MACRO(once)=0,1) // for(;MACRO(once);MACRO(once)=0)
#endif

#ifndef PERROR // extended perror() that prints file:line too
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

#define alloca_      ifdef(V3_CL, _alloca, alloca)
#define atoi64_      ifdef(V3_CL, _atoi64, atoll/*atoi64*/)
#define chdir_       ifdef(V3_CL, _chdir, chdir)
#define flockfile_   ifdef(V3_CL, _lock_file, ifdef(V3_MINGW,_lock_file,flockfile))
#define ftruncate_   ifdef(V3_CL, _chsize_s, ftruncate)
#define funlockfile_ ifdef(V3_CL, _unlock_file, ifdef(V3_MINGW,_unlock_file,funlockfile))
#define mkdir_(p)    ifdef(V3_WINDOWS, mkdir(p), mkdir(p,0777)) // @todo: win32 function is recursive, probably counterparts should be as well
#define msize_       ifdef(1, sys_msize)
#define pclose_      ifdef(V3_CL, _pclose, pclose)
#define popen_       ifdef(V3_CL, _popen, popen)
#define realloc_     ifdef(1, sys_realloc)
#define strcmpi_     ifdef(V3_WINDOWS, _strcmpi, strcasecmp)
#define strncmpi_    ifdef(V3_WINDOWS, strnicmp, strncasecmp)
#define threadlocal_ ifdef(V3_CL, __declspec(thread), __thread)
#define unlink_      ifdef(V3_WINDOWS, _unlink, unlink)

#  if !defined PATH_MAX && defined MAX_PATH
#define PATH_MAX MAX_PATH
#elif !defined MAX_PATH && defined PATH_MAX
#define MAX_PATH PATH_MAX
#else
#define MAX_PATH 260
#define PATH_MAX 260
#endif

#include ifdef(V3_BSD, <malloc/malloc.h>, <malloc.h>) // freebsd: <malloc_np.h>
ifdef(V3_ANDROID, size_t dlmalloc_usable_size(void*));

static void*  (*sys_realloc)(void*, size_t) = realloc;
static size_t (*sys_msize)(void *) = ifdef(V3_BSD, (size_t (*)(void *))malloc_size, ifdef(V3_WINDOWS, _msize, ifdef(V3_ANDROID, dlmalloc_usable_size, malloc_usable_size)));

void* zrealloc(void*, size_t);

// this block is intentionally at bottom, so more devs find what we do here.
// we redirect all realloc > zrealloc calls, so:
// - common code path for all allocations (we only use realloc; no malloc,calloc,free).
// - all our allocs are zero-cleared (zrealloc). especially while enlarging buffers.

#define realloc      zrealloc
