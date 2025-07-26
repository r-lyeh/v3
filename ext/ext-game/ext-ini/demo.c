#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
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

    return 0;
}
