#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    // dump contents of embed/test.txt file, which should be embedded into executable if compiled via makefile
    if( EMBED_LEN(test_txt) )
    printf("contents of `embed/test.txt` file as follows: %.*s\n", EMBED_LEN(test_txt), EMBED_BIN(test_txt));
    else
    printf("cannot find embedded resource. forgot to compile via makefile?\n");
    return 0;
}
