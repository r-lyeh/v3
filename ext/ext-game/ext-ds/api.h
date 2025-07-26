#if CODE
#pragma once
#define STB_DS_IMPLEMENTATION
#endif

#include "3rd/stb_ds.h"

#if __GNUC__ || __clang__ || _MSC_VER // vs22
#undef  STBDS_ADDRESSOF
#define STBDS_ADDRESSOF(typevar, value)     ((__typeof__(typevar)[1]){value}) // literal array decays to pointer to value
#endif

#ifndef DS_H
#define DS_H

#include "map.h"
#include "set.h"
#include "array.h"

#endif

#include "quark.h"
