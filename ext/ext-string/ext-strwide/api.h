extern wchar_t* (*strwide)( const char* input );   // utf8 (used everywhere) to wstring (used by win32)
extern char* (*strshort)( const wchar_t* input ); // wstring (used by win32) to utf8 (used everywhere)

#if CODE
#pragma once
#include v3_text_backend

wchar_t* (*strwide)( const char* input ) = strwide_;
char* (*strshort)( const wchar_t* input ) = strshort_;

#endif
