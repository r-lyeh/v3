#define V3_IMPLEMENTATION
#include "v3.h"

float divf(float a, float b) {
    error = 0;
    if( !b ) return error = "500: cannot divide by 0", NAN;
    return a / b;
}
AUTORUN {
    float d = divf(3, 0);
    assert(d != d);
    assert(error);
    assert(0 == strcmp(error, "500: cannot divide by 0"));
    assert(500 == atoi(error));
}
int main() { return 0; }
