// v3 framework
// - rlyeh, 0MIT licensed

// -----------------------------------------------------------------------------
// available backends. pick your poisons:

#define v3_alert_backend      ifdef(true,    "alert.h")
#define v3_app_backend        ifdef(true,    "app_rgfw.h",      "app_null.h")
#define v3_audio_backend      ifdef(true,    "audio.h",         "audio_null.h")
#define v3_battery_backend    ifdef(true,    "battery_rabia.h")
#define v3_beep_backend       ifdef(true,    "beep.h")
#define v3_breakpoint_backend ifdef(true,    "breakpoint.h")
#define v3_cook_backend       ifdef(retail,  "cook_null.h",     "cook.h")
#define v3_die_backend        ifdef(true,    "die.h")
#define v3_dll_backend        ifdef(true,    "dll.h")
#define v3_file_backend       ifdef(true,    "file_stdio.h",    "file_null.h")
#define v3_flag_backend       ifdef(true,    "flag.h")
#define v3_gui_backend        ifdef(retail,  "gui_null.h",      "gui_microui.h")
#define v3_hexdump_backend    ifdef(true,    "hexdump.h")
#define v3_image_backend      ifdef(true,    "image_stb.h")
#define v3_input_backend      ifdef(true,    "input_rgfw.h",    "input_null.h")
#define v3_log_backend        ifdef(retail,  "log_null.h",      "log_ansi.h")
#define v3_lookup_backend     ifdef(true,    "lookup_dir.h",    "lookup_null.h")
#define v3_ls_backend         ifdef(true,    "ls_native.h")
#define v3_memleaks_backend   ifdef(retail,  "memleaks_null.h", "memleaks_file.h")
#define v3_object_backend     ifdef(true,    "object.h")
#define v3_render_backend     ifdef(true,    "render_gl3.h")
#define v3_resize_backend     ifdef(true,    "resize_realloc.h")
#define v3_script_backend     ifdef(true,    "script_lua548.h", "script_null.h")
#define v3_path_backend       ifdef(true,    "path_stat.h")
#define v3_test_backend       ifdef(retail,  "test_null.h",     "test_stdio.h")
#define v3_text_backend       ifdef(true,    "text_widen.h")
#define v3_time_backend       ifdef(true,    "time_native.h",   "time_null.h")
#define v3_trace_backend      ifdef(true,    "trace.h")
#define v3_va_backend         ifdef(true,    "va_stb.h")
#define v3_vfs_backend        ifdef(true,    "vfs_zip.h",       "vfs_null.h")

// -----------------------------------------------------------------------------
// defines, macros and system headers we commonly use
#include "v3_def.h"

// include module headers
#include "ext/ext.h"

// unroll module implementations
#ifdef V3_IMPLEMENTATION
#define CODE 1
#include "ext/ext.h"
#endif

#pragma once
