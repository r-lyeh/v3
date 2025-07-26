extern wchar_t* (*widen)( const char* input );   // utf8 (used everywhere) to wstring (used by win32)
extern char* (*shorten)( const wchar_t* input ); // wstring (used by win32) to utf8 (used everywhere)

#if CODE
#pragma once
#include v3_text_backend

wchar_t* (*widen)( const char* input ) = widen_;
char* (*shorten)( const wchar_t* input ) = shorten_;

#endif
