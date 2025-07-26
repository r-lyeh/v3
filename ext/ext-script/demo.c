#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    puts("v3, hello from C");

    for(script_t *L = script_create(); L; script_destroy(L), L = 0)
        script_runfile(L, lookup("hello.lua"));

    return 0;
}
