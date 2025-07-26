#if CODE
#pragma once
#define LUA_IMPL
#define l_likely(x)     luai_likely(x)
#define l_unlikely(x)   luai_unlikely(x)
#endif

#pragma push_macro("cast") // our object system
#ifdef  cast
#undef  cast
#endif
#define error error_LUA
#define panic panic_LUA // osx <mach.h>
#include "3rd/minilua.h"
#undef  panic
#undef  error
#pragma pop_macro("cast")
