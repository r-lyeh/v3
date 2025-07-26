#if CODE
#pragma once
#define XINCBIN_IMPLEMENTATION
#endif

#ifndef INCBIN_SILENCE_BITCODE_WARNING
#define INCBIN_SILENCE_BITCODE_WARNING
#endif

#include "3rd/xincbin.h"

#if __has_include("../../v3.rc") && __has_include("../../v3.res") && defined _MSC_VER
#include "../../v3.rc"
#define EMBED_BIN(x) XINCBIN_GET(x).data
#define EMBED_LEN(x) XINCBIN_GET(x).size
#elif __has_include("../../v3.rc") && !defined _MSC_VER
#include "../../v3.rc"
#define EMBED_BIN(x) XINCBIN_GET(x).data
#define EMBED_LEN(x) XINCBIN_GET(x).size
#else
#define EMBED_BIN(x) ""
#define EMBED_LEN(x) 0
#endif
