extern void (*hexdump)( const void *ptr, unsigned len );
extern void (*hexdumpf)( FILE *fp, const void *ptr, unsigned len, int width );

#if CODE
#pragma once
#include v3_hexdump_backend

void (*hexdump)( const void *ptr, unsigned len ) = hexdump_;
void (*hexdumpf)( FILE *fp, const void *ptr, unsigned len, int width ) = hexdumpf_;

#endif
