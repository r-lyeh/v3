// v3 framework
// - rlyeh, PD/0BSD/MIT0/uL licensed

// ----------------------------------------------------------------------------
// visibility
// win32 users may need to use -DV3_API=V3_EXPORT/V3_IMPORT/V3_STATIC as needed when using DLLs.

#define V3_IMPORT ifdef(V3_WINDOWS, ifdef(V3_GCC, __attribute__ ((dllimport)), __declspec(dllimport)))
#define V3_EXPORT ifdef(V3_WINDOWS, ifdef(V3_GCC, __attribute__ ((dllexport)), __declspec(dllexport)))
#define V3_STATIC

#ifndef V3_API
#define V3_API    V3_EXPORT // public visibility by default
#endif

// -----------------------------------------------------------------------------
// available backends. pick your poisons:

#define v3_alert_backend      ifdef(true,       "alert().h")
#define v3_app_backend        ifdef(true,       "app(rgfw).h",       "app(null).h")
#define v3_asset_backend      ifdef(true,       "asset().h")
#define v3_audio_backend      ifdef(true,       "audio().h",         "audio(null).h")
#define v3_battery_backend    ifdef(true,       "battery(rabia).h")
#define v3_beep_backend       ifdef(true,       "beep().h")
#define v3_breakpoint_backend ifdef(true,       "breakpoint().h")
#define v3_callback_backend   ifdef(true,       "callback().h")
#define v3_cook_backend       ifdef(V3_RETAIL,  "cook(null).h",      "cook().h")
#define v3_dd_backend         ifdef(true,       "dd(gl3).h")
#define v3_die_backend        ifdef(true,       "die().h")
#define v3_dll_backend        ifdef(true,       "dll().h")
#define v3_eval_backend       ifdef(true,       "eval(stoop).h")
#define v3_file_backend       ifdef(true,       "file(stdio).h",     "file(null).h")
#define v3_flag_backend       ifdef(true,       "flag().h")
#define v3_fpslimit_backend   ifdef(true,       "fps(auto).h",       "fps(max).h", "fps(60).h", "fps(30).h")
#define v3_gamepad_backend    ifdef(true,       "gamepad(glfw3).h",  "gamepad(mg).h",     "gamepad(null).h")
#define v3_gldebug_backend    ifdef(V3_RETAIL,  "gldebug(null).h",   "gldebug().h")
#define v3_gui_backend        ifdef(V3_RETAIL,  "gui(null).h",       "gui(microui).h")
#define v3_hexdump_backend    ifdef(true,       "hexdump().h")
#define v3_image_backend      ifdef(true,       "image(stb).h")
#define v3_ini_backend        ifdef(true,       "ini().h")
#define v3_json_backend       ifdef(true,       "json().h")
#define v3_keyboard_backend   ifdef(true,       "keyboard(glfw3).h", "keyboard(null).h")
#define v3_log_backend        ifdef(V3_RETAIL,  "log(null).h",       "log(ansi).h")
#define v3_lookup_backend     ifdef(true,       "lookup().h")
#define v3_ls_backend         ifdef(true,       "ls().h")
#define v3_math_backend       ifdef(true,       "vecmath().h")
#define v3_memleaks_backend   ifdef(V3_RETAIL,  "memleaks(null).h",  "memleaks(file).h")
#define v3_monitor_backend    ifdef(true,       "monitor(glfw3).h",  "monitor(rgfw).h")
#define v3_mouse_backend      ifdef(true,       "mouse(glfw3).h",    "mouse(null).h")
#define v3_object_backend     ifdef(true,       "object().h")
#define v3_path_backend       ifdef(true,       "path(stat).h")
#define v3_render_backend     ifdef(true,       "render(gl3).h")
#define v3_resize_backend     ifdef(true,       "resize(realloc).h")
#define v3_script_backend     ifdef(true,       "script(lua548).h",  "script(null).h")
#define v3_stream_backend     ifdef(true,       "stream().h")
#define v3_test_backend       ifdef(V3_RETAIL,  "test(null).h",      "test(stdio).h")
#define v3_text_backend       ifdef(true,       "text(widen).h")
#define v3_thread_backend     ifdef(true,       "thread(mt).h")
#define v3_time_backend       ifdef(true,       "time().h",          "time(null).h")
#define v3_trace_backend      ifdef(true,       "trace().h")
#define v3_url_backend        ifdef(true,       "url().h")
#define v3_va_backend         ifdef(true,       "va(stb).h")
#define v3_vfs_backend        ifdef(true,       "vfs(zip).h",        "vfs(null).h")
#define v3_xml_backend        ifdef(true,       "xml().h")

// ----------------------------------------------------------------------------
// some required defines before any other header is included
// ----------------------------------------------------------------------------

// #define _POSIX_C_SOURCE 199309L

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

#ifndef UNICODE
#define UNICODE
#endif

// ----------------------------------------------------------------------------
// standard C headers we commonly use in all platforms
// ----------------------------------------------------------------------------

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
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

// -----------------------------------------------------------------------------
// glue code
// ----------------------------------------------------------------------------

#define alloca_      ifdef(V3_CL, _alloca, alloca)
#define atoi64_      ifdef(V3_CL, _atoi64, atoll/*atoi64*/)
#define chdir_       ifdef(V3_CL, _chdir, chdir)
#define flockfile_   ifdef(V3_CL, _lock_file, ifdef(V3_MINGW,_lock_file,flockfile))
#define ftruncate_   ifdef(V3_CL, _chsize_s, ftruncate)
#define funlockfile_ ifdef(V3_CL, _unlock_file, ifdef(V3_MINGW,_unlock_file,funlockfile))
#define mkdir_(p)    ifdef(V3_WINDOWS, mkdir(p), mkdir(p,0777)) // @todo: win32 function is recursive, probably counterparts should be as well
#define pclose_      ifdef(V3_CL, _pclose, pclose)
#define popen_       ifdef(V3_CL, _popen, popen)
#define restrict_    ifdef(V3_WINDOWS, __restrict, __restrict__)
#define strcmpi_     ifdef(V3_WINDOWS, _strcmpi, strcasecmp)
#define strncmpi_    ifdef(V3_WINDOWS, strnicmp, strncasecmp)
#define threadlocal_ ifdef(V3_CL, __declspec(thread), __thread)
#define unlink_      ifdef(V3_WINDOWS, _unlink, unlink)

// ----------------------------------------------------------------------------

#define V3_VERSION    100

#ifndef __cplusplus
#define V3_C          1
#define V3_CPP        0
#else
#define V3_C          0
#define V3_CPP        1
#endif

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define V3_BIG        0
#define V3_LITTLE     1
#else
#define V3_BIG        1
#define V3_LITTLE     0
#endif

#if defined(_M_IA64) || defined(__ia64)
#define V3_IA64       1
#else
#define V3_IA64       0
#endif

#if defined(_M_X64) || (defined(__x86_64__) && !defined(__ILP32__))
#define V3_X64        1
#define V3_X86        0
#elif defined(_M_IX86) || defined(__i386) || defined(__i386__)
#define V3_X64        0
#define V3_X86        1
#else
#define V3_X64        0
#define V3_X86        0
#endif

#if defined(_M_ARM64) || defined(__arm64) || defined(__arm64__) || defined(__aarch64__)
#define V3_ARM64      1
#define V3_ARM32      0
#elif defined(_M_ARM) || defined(__arm__)
#define V3_ARM64      0
#define V3_ARM32      1
#else
#define V3_ARM64      0
#define V3_ARM32      0
#endif

#if defined(_ARCH_PPC64) || defined(__PPC64__) || defined(__ppc64__) || defined(__powerpc64__)
#define V3_PPC64      1
#define V3_PPC32      0
#elif defined(_ARCH_PPC) || defined(__PPC__) || defined(__ppc__) || defined(__powerpc__) || defined(__ppc) || defined(__powerpc)
#define V3_PPC64      0
#define V3_PPC32      1
#else
#define V3_PPC64      0
#define V3_PPC32      0
#endif

#if V3_IA64 || V3_X64 || V3_ARM64 || V3_PPC64
#define V3_64         1
#define V3_32         0
#else
#define V3_64         0
#define V3_32         1
#endif

#if defined _WIN32
#define V3_WINDOWS    1
#else
#define V3_WINDOWS    0
#endif

#if defined __linux__ || defined __linux
#define V3_LINUX      1
#else
#define V3_LINUX      0
#endif

#if defined __APPLE__
#define V3_MACOS      1
#else
#define V3_MACOS      0
#endif

#if defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__ || defined __APPLE__
#define V3_BSD        1
#else
#define V3_BSD        0
#endif

#if defined __EMSCRIPTEN__
#define V3_WASM       1
#else
#define V3_WASM       0
#endif

#if defined __ANDROID_API__
#define V3_ANDROID    1
#else
#define V3_ANDROID    0
#endif

#if V3_WINDOWS || V3_ANDROID || V3_WASM
#define V3_POSIX      0
#else
#define V3_POSIX      1
#endif

#  if defined __TINYC__
#define V3_TCC        1
#define V3_GCC        0
#elif defined __GNUC__
#define V3_TCC        0
#define V3_GCC        1
#else
#define V3_TCC        0
#define V3_GCC        0
#endif

#if defined __clang__ && defined _MSC_VER
#define V3_CLANG      1
#define V3_CLANGCL    1
#elif defined __clang__
#define V3_CLANG      1
#define V3_CLANGCL    0
#else
#define V3_CLANG      0
#define V3_CLANGCL    0
#endif

#if defined _MSC_VER
#define V3_CL         1
#define V3_MINGW      0
#elif defined __MINGW64__ || defined __MINGW32__
#define V3_CL         0
#define V3_MINGW      1
#else
#define V3_CL         0
#define V3_MINGW      0
#endif

// rely on NDEBUG as the official/portable way to disable asserts
// we extend NDEBUG=[0,1,2,3] to signal the compiler optimization flags O0,O1,O2,O3
#if   defined NDEBUG && NDEBUG >= 2
#define V3_DEBUG      0
#define V3_RELEASE    0
#define V3_RETAIL     1
#elif defined NDEBUG && NDEBUG >= 1
#define V3_DEBUG      0
#define V3_RELEASE    1
#define V3_RETAIL     0
#else
#define V3_DEBUG      1
#define V3_RELEASE    0
#define V3_RETAIL     0
#endif

#ifdef _USRDLL // detect /LD or /DLL flag (cl)
#define V3_DLL 1
#define V3_EXE 0
#else
#define V3_DLL 0
#define V3_EXE 1
#endif

#ifdef _DLL // detect /MD flag (cl CRT)
#define V3_MD 1
#define V3_MT 0
#else
#define V3_MD 0
#define V3_MT 1
#endif

// ----------------------------------------------------------------------------
// if/n/def hell
// ----------------------------------------------------------------------------

#define ifdef(arg, then, /*else*/...)        ifd3f(arg, then, __VA_ARGS__)
#define ifd3f(arg, then, /*else*/...)        ifdef_##arg(then, ##__VA_ARGS__)

#define ifndef(arg, then, /*else*/...)       ifnd3f(arg, then, __VA_ARGS__)
#define ifnd3f(arg, then, /*else*/...)       ifdef_##arg(__VA_ARGS__, then)

#define ifdef_1(then, /*else*/...)           then
#define ifdef_0(then, /*else*/...)           __VA_ARGS__

#define ifdef_true(then, /*else*/...)        then
#define ifdef_false(then, /*else*/...)       __VA_ARGS__

// -----------------------------------------------------------------------------

#if V3_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winitializer-overrides"
#pragma clang diagnostic ignored "-Wdeprecated-non-prototype"
#elif V3_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverride-init" // gcc >= 6
#pragma GCC diagnostic ignored "-Woverride-init-side-effects" // gcc >= 6
#elif V3_CL
#pragma warning(disable : 4010) // backslash \ in //commented multi-line
#endif

// -----------------------------------------------------------------------------
// include module headers
// -----------------------------------------------------------------------------
#if __has_include("ext/ext.h")
#include "ext/ext.h"
#endif

// -----------------------------------------------------------------------------
// include os headers
// -----------------------------------------------------------------------------
#include ifdef(V3_WINDOWS, <winsock2.h>, <unistd.h>)
#include ifdef(V3_WINDOWS, <winsock2.h>, <dlfcn.h>)

// -----------------------------------------------------------------------------
// unroll module implementations
// -----------------------------------------------------------------------------
#ifdef V3_IMPLEMENTATION
#define CODE 1
#if __has_include("ext/ext.h")
#include "ext/ext.h"
#endif
#endif

#pragma once
