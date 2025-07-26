// v3 framework
// - rlyeh, PD/0BSD/MIT0/uL licensed

// -----------------------------------------------------------------------------
// available backends. pick your poisons:

#define v3_alert_backend      ifdef(true,       "alert().h")
#define v3_app_backend        ifdef(true,       "app(rgfw).h",       "app(null).h")
#define v3_audio_backend      ifdef(true,       "audio().h",         "audio(null).h")
#define v3_battery_backend    ifdef(true,       "battery(rabia).h")
#define v3_beep_backend       ifdef(true,       "beep().h")
#define v3_breakpoint_backend ifdef(true,       "breakpoint().h")
#define v3_callback_backend   ifdef(true,       "callback().h")
#define v3_cook_backend       ifdef(V3_RETAIL,  "cook(null).h",      "cook().h")
#define v3_die_backend        ifdef(true,       "die().h")
#define v3_dll_backend        ifdef(true,       "dll().h")
#define v3_url_backend        ifdef(true,       "url().h")
#define v3_eval_backend       ifdef(true,       "eval(stoop).h")
#define v3_file_backend       ifdef(true,       "file(stdio).h",     "file(null).h")
#define v3_fpslimit_backend   ifdef(true,       "fps(auto).h",       "fps(max).h", "fps(60).h", "fps(30).h")
#define v3_flag_backend       ifdef(true,       "flag().h")
#define v3_lookup_backend     ifdef(true,       "lookup().h")
#define v3_gamepad_backend    ifdef(true,       "gamepad(mg).h",     "gamepad(null).h")
#define v3_gldebug_backend    ifdef(V3_RETAIL,  "gldebug(null).h",   "gldebug().h")
#define v3_gui_backend        ifdef(V3_RETAIL,  "gui(null).h",       "gui(microui).h")
#define v3_hexdump_backend    ifdef(true,       "hexdump().h")
#define v3_image_backend      ifdef(true,       "image(stb).h")
#define v3_input_backend      ifdef(true,       "input(rgfw).h",     "input(null).h")
#define v3_log_backend        ifdef(V3_RETAIL,  "log(null).h",       "log(ansi).h")
#define v3_ls_backend         ifdef(true,       "ls().h")
#define v3_math_backend       ifdef(true,       "vecmath().h")
#define v3_memleaks_backend   ifdef(V3_RETAIL,  "memleaks(null).h",  "memleaks(file).h")
#define v3_object_backend     ifdef(true,       "object().h")
#define v3_path_backend       ifdef(true,       "path(stat).h")
#define v3_render_backend     ifdef(true,       "render(gl3).h")
#define v3_resize_backend     ifdef(true,       "resize(realloc).h")
#define v3_script_backend     ifdef(true,       "script(lua548).h",  "script(null).h")
#define v3_test_backend       ifdef(V3_RETAIL,  "test(null).h",      "test(stdio).h")
#define v3_text_backend       ifdef(true,       "text(widen).h")
#define v3_thread_backend     ifdef(true,       "thread(mt).h")
#define v3_time_backend       ifdef(true,       "time().h",          "time(null).h")
#define v3_trace_backend      ifdef(true,       "trace().h")
#define v3_va_backend         ifdef(true,       "va(stb).h")
#define v3_vfs_backend        ifdef(true,       "vfs(zip).h",        "vfs(null).h")

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
// defines, macros and standard headers we commonly use
#include "v3_def.h"

// include module headers
#include "ext/ext.h"

// include os headers
#include ifdef(V3_WINDOWS, <winsock2.h>, <unistd.h>)
#include ifdef(V3_WINDOWS, <winsock2.h>, <dlfcn.h>)

// unroll module implementations
#ifdef V3_IMPLEMENTATION
#define CODE 1
#include "ext/ext.h"
#endif

#pragma once
