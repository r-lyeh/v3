#ifndef INCBIN_H
#define INCBIN_H

/* Stringize */
#define INCBIN_STR(X) \
    #X
#define INCBIN_STRINGIZE(X) \
    INCBIN_STR(X)
/* Concatenate */
#define INCBIN_CAT(X, Y) \
    X ## Y
#define INCBIN_CONCATENATE(X, Y) \
    INCBIN_CAT(X, Y)

#define INCBIN_PREFIX xincbin_

#ifndef RC_INVOKED
// Copied from INCBIN
/**
 * @file incbin.h
 * @author Dale Weiler
 * @brief Utility for including binary files
 *
 * Facilities for including binary files into the current translation unit and
 * making use from them externally in other translation units.
 */
#include <limits.h>
#include <stddef.h>

#if   defined(__AVX512BW__) || \
      defined(__AVX512CD__) || \
      defined(__AVX512DQ__) || \
      defined(__AVX512ER__) || \
      defined(__AVX512PF__) || \
      defined(__AVX512VL__) || \
      defined(__AVX512F__)
# define INCBIN_ALIGNMENT_INDEX 6
#elif defined(__AVX__)      || \
      defined(__AVX2__)
# define INCBIN_ALIGNMENT_INDEX 5
#elif defined(__SSE__)      || \
      defined(__SSE2__)     || \
      defined(__SSE3__)     || \
      defined(__SSSE3__)    || \
      defined(__SSE4_1__)   || \
      defined(__SSE4_2__)   || \
      defined(__neon__)     || \
      defined(__ARM_NEON)   || \
      defined(__ALTIVEC__)
# define INCBIN_ALIGNMENT_INDEX 4
#elif ULONG_MAX != 0xffffffffu
# define INCBIN_ALIGNMENT_INDEX 3
# else
# define INCBIN_ALIGNMENT_INDEX 2
#endif

/* Lookup table of (1 << n) where `n' is `INCBIN_ALIGNMENT_INDEX' */
#define INCBIN_ALIGN_SHIFT_0 1
#define INCBIN_ALIGN_SHIFT_1 2
#define INCBIN_ALIGN_SHIFT_2 4
#define INCBIN_ALIGN_SHIFT_3 8
#define INCBIN_ALIGN_SHIFT_4 16
#define INCBIN_ALIGN_SHIFT_5 32
#define INCBIN_ALIGN_SHIFT_6 64

/* Actual alignment value */
#define INCBIN_ALIGNMENT \
    INCBIN_CONCATENATE( \
        INCBIN_CONCATENATE(INCBIN_ALIGN_SHIFT, _), \
        INCBIN_ALIGNMENT_INDEX)

/* Deferred macro expansion */
#define INCBIN_EVAL(X) \
    X
#define INCBIN_INVOKE(N, ...) \
    INCBIN_EVAL(N(__VA_ARGS__))
/* Variable argument count for overloading by arity */
#define INCBIN_VA_ARG_COUNTER(_1, _2, _3, N, ...) N
#define INCBIN_VA_ARGC(...) INCBIN_VA_ARG_COUNTER(__VA_ARGS__, 3, 2, 1, 0)

/* Green Hills uses a different directive for including binary data */
#if defined(__ghs__)
#  if (__ghs_asm == 2)
#    define INCBIN_MACRO ".file"
/* Or consider the ".myrawdata" entry in the ld file */
#  else
#    define INCBIN_MACRO "\tINCBIN"
#  endif
#else
#  define INCBIN_MACRO ".incbin"
#endif

#ifndef _MSC_VER
#  define INCBIN_ALIGN \
    __attribute__((aligned(INCBIN_ALIGNMENT)))
#else
#  define INCBIN_ALIGN __declspec(align(INCBIN_ALIGNMENT))
#endif

#if defined(__arm__) || /* GNU C and RealView */ \
    defined(__arm) || /* Diab */ \
    defined(_ARM) /* ImageCraft */
#  define INCBIN_ARM
#endif

#ifdef __GNUC__
/* Utilize .balign where supported */
#  define INCBIN_ALIGN_HOST ".balign " INCBIN_STRINGIZE(INCBIN_ALIGNMENT) "\n"
#  define INCBIN_ALIGN_BYTE ".balign 1\n"
#elif defined(INCBIN_ARM)
/*
 * On arm assemblers, the alignment value is calculated as (1 << n) where `n' is
 * the shift count. This is the value passed to `.align'
 */
#  define INCBIN_ALIGN_HOST ".align " INCBIN_STRINGIZE(INCBIN_ALIGNMENT_INDEX) "\n"
#  define INCBIN_ALIGN_BYTE ".align 0\n"
#else
/* We assume other inline assembler's treat `.align' as `.balign' */
#  define INCBIN_ALIGN_HOST ".align " INCBIN_STRINGIZE(INCBIN_ALIGNMENT) "\n"
#  define INCBIN_ALIGN_BYTE ".align 1\n"
#endif

/* INCBIN_CONST is used by incbin.c generated files */
#if defined(__cplusplus)
#  define INCBIN_EXTERNAL extern "C"
#  define INCBIN_CONST    extern const
#else
#  define INCBIN_EXTERNAL extern
#  define INCBIN_CONST    const
#endif

/**
 * @brief Optionally override the linker section into which size and data is
 * emitted.
 *
 * @warning If you use this facility, you might have to deal with
 * platform-specific linker output section naming on your own.
 */
#if !defined(INCBIN_OUTPUT_SECTION)
#  if defined(__APPLE__)
#    define INCBIN_OUTPUT_SECTION ".const_data"
#  else
#    define INCBIN_OUTPUT_SECTION ".rodata"
#  endif
#endif

/**
 * @brief Optionally override the linker section into which data is emitted.
 *
 * @warning If you use this facility, you might have to deal with
 * platform-specific linker output section naming on your own.
 */
#if !defined(INCBIN_OUTPUT_DATA_SECTION)
#  define INCBIN_OUTPUT_DATA_SECTION INCBIN_OUTPUT_SECTION
#endif

/**
 * @brief Optionally override the linker section into which size is emitted.
 *
 * @warning If you use this facility, you might have to deal with
 * platform-specific linker output section naming on your own.
 *
 * @note This is useful for Harvard architectures where program memory cannot
 * be directly read from the program without special instructions. With this you
 * can chose to put the size variable in RAM rather than ROM.
 */
#if !defined(INCBIN_OUTPUT_SIZE_SECTION)
#  define INCBIN_OUTPUT_SIZE_SECTION INCBIN_OUTPUT_SECTION
#endif

#if defined(__APPLE__)
#  include "TargetConditionals.h"
#  if defined(TARGET_OS_IPHONE) && !defined(INCBIN_SILENCE_BITCODE_WARNING)
#    warning "incbin is incompatible with bitcode. Using the library will break upload to App Store if you have bitcode enabled. Add `#define INCBIN_SILENCE_BITCODE_WARNING` before including this header to silence this warning."
#  endif
/* The directives are different for Apple branded compilers */
#  define INCBIN_SECTION         INCBIN_OUTPUT_SECTION "\n"
#  define INCBIN_GLOBAL(NAME)    ".globl " INCBIN_MANGLE INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME "\n"
#  define INCBIN_INT             ".long "
#  define INCBIN_MANGLE          "_"
#  define INCBIN_BYTE            ".byte "
#  define INCBIN_TYPE(...)
#else
#  define INCBIN_SECTION         ".section " INCBIN_OUTPUT_SECTION "\n"
#  define INCBIN_GLOBAL(NAME)    ".global " INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME "\n"
#  if defined(__ghs__)
#    define INCBIN_INT           ".word "
#  else
#    define INCBIN_INT           ".int "
#  endif
#  if defined(__USER_LABEL_PREFIX__)
#    define INCBIN_MANGLE        INCBIN_STRINGIZE(__USER_LABEL_PREFIX__)
#  else
#    define INCBIN_MANGLE        ""
#  endif
#  if defined(INCBIN_ARM)
/* On arm assemblers, `@' is used as a line comment token */
#    define INCBIN_TYPE(NAME)    ".type " INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME ", %object\n"
#  elif defined(__MINGW32__) || defined(__MINGW64__)
/* Mingw doesn't support this directive either */
#    define INCBIN_TYPE(NAME)
#  else
/* It's safe to use `@' on other architectures */
#    define INCBIN_TYPE(NAME)    ".type " INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME ", @object\n"
#  endif
#  define INCBIN_BYTE            ".byte "
#endif

/* List of style types used for symbol names */
#define INCBIN_STYLE_CAMEL 0
#define INCBIN_STYLE_SNAKE 1

/**
 * @brief Specify the style used for symbol names.
 *
 * Possible options are
 * - INCBIN_STYLE_CAMEL "CamelCase"
 * - INCBIN_STYLE_SNAKE "snake_case"
 *
 * @note By default this is INCBIN_STYLE_CAMEL
 *
 * @code
 * #define INCBIN_STYLE INCBIN_STYLE_SNAKE
 * #include "incbin.h"
 * INCBIN(foo, "foo.txt");
 *
 * // Now you have the following symbols:
 * // const unsigned char <prefix>foo_data[];
 * // const unsigned char *const <prefix>foo_end;
 * // const unsigned int <prefix>foo_size;
 * @endcode
 */
#if !defined(INCBIN_STYLE)
#  define INCBIN_STYLE INCBIN_STYLE_SNAKE
#endif

/* Style lookup tables */
#define INCBIN_STYLE_0_DATA Data
#define INCBIN_STYLE_0_END End
#define INCBIN_STYLE_0_SIZE Size
#define INCBIN_STYLE_1_DATA _data
#define INCBIN_STYLE_1_END _end
#define INCBIN_STYLE_1_SIZE _size

/* Style lookup: returning identifier */
#define INCBIN_STYLE_IDENT(TYPE) \
    INCBIN_CONCATENATE( \
        INCBIN_STYLE_, \
        INCBIN_CONCATENATE( \
            INCBIN_EVAL(INCBIN_STYLE), \
            INCBIN_CONCATENATE(_, TYPE)))

/* Style lookup: returning string literal */
#define INCBIN_STYLE_STRING(TYPE) \
    INCBIN_STRINGIZE( \
        INCBIN_STYLE_IDENT(TYPE)) \

/* Generate the global labels by indirectly invoking the macro with our style
 * type and concatenating the name against them. */
#define INCBIN_GLOBAL_LABELS(NAME, TYPE) \
    INCBIN_INVOKE( \
        INCBIN_GLOBAL, \
        INCBIN_CONCATENATE( \
            NAME, \
            INCBIN_INVOKE( \
                INCBIN_STYLE_IDENT, \
                TYPE))) \
    INCBIN_INVOKE( \
        INCBIN_TYPE, \
        INCBIN_CONCATENATE( \
            NAME, \
            INCBIN_INVOKE( \
                INCBIN_STYLE_IDENT, \
                TYPE)))

#define INCBIN_EXTERN(TYPE, NAME) \
    INCBIN_EXTERNAL const INCBIN_ALIGN TYPE \
        INCBIN_CONCATENATE( \
            INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
            INCBIN_STYLE_IDENT(DATA))[]; \
    INCBIN_EXTERNAL const INCBIN_ALIGN TYPE *const \
    INCBIN_CONCATENATE( \
        INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
        INCBIN_STYLE_IDENT(END)); \
    INCBIN_EXTERNAL const unsigned int \
        INCBIN_CONCATENATE( \
            INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), \
            INCBIN_STYLE_IDENT(SIZE))

#  define INCBIN_COMMON(TYPE, NAME, FILENAME, TERMINATOR) \
    __asm__(INCBIN_SECTION \
            INCBIN_GLOBAL_LABELS(NAME, DATA) \
            INCBIN_ALIGN_HOST \
            INCBIN_MANGLE INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME INCBIN_STYLE_STRING(DATA) ":\n" \
            INCBIN_MACRO " \"" FILENAME "\"\n" \
                TERMINATOR \
            INCBIN_GLOBAL_LABELS(NAME, END) \
            INCBIN_ALIGN_BYTE \
            INCBIN_MANGLE INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME INCBIN_STYLE_STRING(END) ":\n" \
                INCBIN_BYTE "1\n" \
            INCBIN_GLOBAL_LABELS(NAME, SIZE) \
            INCBIN_ALIGN_HOST \
            INCBIN_MANGLE INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME INCBIN_STYLE_STRING(SIZE) ":\n" \
                INCBIN_INT INCBIN_MANGLE INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME INCBIN_STYLE_STRING(END) " - " \
                           INCBIN_MANGLE INCBIN_STRINGIZE(INCBIN_PREFIX) #NAME INCBIN_STYLE_STRING(DATA) "\n" \
            INCBIN_ALIGN_HOST \
            ".text\n" \
    )

typedef struct xincbin_data_s {
    unsigned int size;
    const unsigned char* data;
} xincbin_data_t;

#ifdef _MSC_VER
#	define XINCBIN(NAME, FILENAME) INCBIN_EXTERNAL const char* INCBIN_CONCATENATE(INCBIN_PREFIX, NAME);
#	define XINCBIN_GET(NAME) xincbin_get(INCBIN_CONCATENATE(INCBIN_PREFIX, NAME))
	INCBIN_EXTERNAL xincbin_data_t xincbin_get(const char* name);
#else
#	define XINCBIN(NAME, FILENAME) INCBIN_EXTERN(unsigned char, NAME);
#	define XINCBIN_GET(NAME) (xincbin_data_t){ \
		.size = INCBIN_CONCATENATE(INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), INCBIN_STYLE_IDENT(SIZE)), \
		.data = INCBIN_CONCATENATE(INCBIN_CONCATENATE(INCBIN_PREFIX, NAME), INCBIN_STYLE_IDENT(DATA)), \
	}
#endif

#else // RC_INVOKED

#define XINCBIN(NAME, FILENAME) INCBIN_CONCATENATE(INCBIN_PREFIX, NAME) RCDATA FILENAME

#endif

#endif // Include guard

#ifdef XINCBIN_IMPLEMENTATION
#pragma once //< @r-lyeh

#undef XINCBIN
#ifdef _MSC_VER
#	define XINCBIN(NAME, FILENAME) const char* INCBIN_CONCATENATE(INCBIN_PREFIX, NAME) = INCBIN_STRINGIZE(INCBIN_CONCATENATE(INCBIN_PREFIX, NAME));
#else
#	define XINCBIN(NAME, FILENAME) INCBIN_COMMON(unsigned char, NAME, FILENAME,);
#endif

#ifdef _MSC_VER

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

xincbin_data_t xincbin_get(const char* name) {
	HMODULE module = NULL;
	GetModuleHandleExA(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
		| GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		name,
		&module
	);
	if (module == NULL) { return (xincbin_data_t) { 0 }; }

	HRSRC res = FindResourceA(module, name, (LPCSTR)RT_RCDATA);
	if (res == NULL) { return (xincbin_data_t){ 0 }; }

	HGLOBAL glob = LoadResource(module, res);
	if (glob == NULL) { return (xincbin_data_t){ 0 }; }

	return (xincbin_data_t){
		.size = (unsigned int)SizeofResource(module, res),
		.data = LockResource(glob),
	};
}

#endif

#endif
