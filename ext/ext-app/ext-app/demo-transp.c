#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    for( app_create(.title = "hello v3", .transparent=1, .scale=50); app_swap(.color = 0x0); ) {}
    return 0;
}
