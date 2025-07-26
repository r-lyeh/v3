#define V3_IMPLEMENTATION
#include "../../v3.h"

int main() {
    assert( eval("1+1") == 2 );         // common path
    assert( eval("1+") != eval("1+") ); // check that errors return NAN
    assert(~puts("Ok") );
    return 0;
}
