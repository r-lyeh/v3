#define V3_IMPLEMENTATION
#include "../v3/v3.h"

int main() {
    // dump contents of embed/test.txt file, which should be embedded into executable if compiled via MAKE.bat
    // @todo: `MAKE embed` command that does embedding but prevents from compiling.
    if( EMBEDDED_LEN(test_txt) )
    printf("contents of `embed/test.txt` file as follows: %.*s\n", EMBEDDED_LEN(test_txt), EMBEDDED_BIN(test_txt));
    else
    printf("cannot find embedded resource. forgot to compile via MAKE.bat?\n");
    return 0;
}
