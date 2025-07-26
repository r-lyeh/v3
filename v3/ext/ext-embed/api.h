#if CODE
#pragma once
#define XINCBIN_IMPLEMENTATION
#endif

#include "xincbin.h"

#if __has_include("../../v3.rc") && __has_include("../../v3.res") && defined _MSC_VER
#include "../../v3.rc"
#define EMBEDDED_BIN(x) XINCBIN_GET(x).data
#define EMBEDDED_LEN(x) XINCBIN_GET(x).size
#elif __has_include("../../v3.rc") && !defined _MSC_VER
#include "../../v3.rc"
#define EMBEDDED_BIN(x) XINCBIN_GET(x).data
#define EMBEDDED_LEN(x) XINCBIN_GET(x).size
#else
#define EMBEDDED_BIN(x) ""
#define EMBEDDED_LEN(x) 0
#endif
