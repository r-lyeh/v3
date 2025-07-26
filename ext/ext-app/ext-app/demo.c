#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    for( app_create(.title = "hello v3"); app_swap(.color = 0xFF4D2DD3); ) {}
    return 0;
}
