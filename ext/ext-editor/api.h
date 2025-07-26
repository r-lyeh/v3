#pragma push_macro("SIZE")
#ifdef SIZE
#undef SIZE
#endif
#define SIZE SIZE2 // windows.h

#include "editor.h"

#pragma pop_macro("SIZE")
