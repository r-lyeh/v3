#include "3rd/microui.h"

#if CODE
#pragma once

#ifdef ABSOLUTE
#undef ABSOLUTE
#endif
#ifdef RELATIVE
#undef RELATIVE
#endif
#include "3rd/microui.c"
#undef unused
#undef expect
#undef push
#undef pop
#include "3rd/microui_renderer.c"

#endif
