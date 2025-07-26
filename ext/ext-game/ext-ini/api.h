// ----------------------------------------------------------------------------
// ini api

// direct access

extern const char* (*ini_read)(const char *pathfile_section_key); // returns NULL if not found. key otherwise
extern const char* (*ini_write)(const char *pathfile_section_key, const char *value); // returns NULL if cannot write. value otherwise

// iteration

extern       int   (*ini_push)(const char *contents);
extern       int   (*ini_pushf)(const char *pathfile);
extern       int    (*ini_count)(void); // returns <0 if not found
extern const char*  (*ini_key)(int id);
extern const char*  (*ini_value)(int id);
extern       int   (*ini_pop)(void);

#if 1
#elif 1
    // append to ini
    ini_write("hello.ini/hello/message", "hello world");

    // read and update ini
    const char *value = ini_read("hello.ini/hello/counter");
    ini_write("hello.ini/hello/counter", va("%d",value?atoi(value)+1:0));

    // iterate ini
    if( ini_push("hello.ini") ) {
        for( int i = 0; i < ini_count(); ++i ) {
            printf("[%s]=`%s`\n", ini_key(i), ini_value(i));
        }
        ini_pop();
    }
#endif

#if CODE
#pragma once
#include v3_ini_backend

const char* (*ini_read)(const char *) = ini_read_;
const char* (*ini_write)(const char *, const char *) = ini_write_;

      int   (*ini_push)(const char *) = ini_push_;
      int   (*ini_pushf)(const char *) = ini_pushf_;
      int    (*ini_count)(void) = ini_count_;
const char*  (*ini_key)(int id) = ini_key_;
const char*  (*ini_value)(int id) = ini_value_;
      int   (*ini_pop)(void) = ini_pop_;
#endif
