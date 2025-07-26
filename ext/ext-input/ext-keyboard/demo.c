#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    for( app_create(); app_swap(); ) {
        #define print_key(X) if(key_down(KEY_##X)) puts(#X " pressed");
        FOREACH_KEY(print_key)
        FOREACH_KEYPAD(print_key)
        FOREACH_KEYLOCAL(print_key)
    }
    return 0;
}