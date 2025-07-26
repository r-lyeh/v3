#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    void *ptr = resize(0, 123);
    // resize(ptr, 0); // do not free(). expect a memory report
    return 0;
}
