// ----------------------------------------------------------------------------
// ini api

// direct access

const char* ini_read(const char *pathfile_section_key); // returns NULL if not found. key otherwise
const char* ini_write(const char *pathfile_section_key, const char *value); // returns NULL if cannot write. value otherwise

// iteration

      int   ini_push(const char *filename);
      int    ini_count(); // returns <0 if not found
const char*  ini_key(int id);
const char*  ini_value(int id);
      int   ini_pop(void);

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
#include "ini().h"
#endif
