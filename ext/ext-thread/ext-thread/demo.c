#define V3_IMPLEMENTATION
#include "v3.h"

void dot(void) {
    while( thread_running(NULL) ) printf(".");
}

int main() {
    thread_t th = thread(.func0 = dot);

    sleep_ss(1);
    
    thread_drop(&th);

    puts("Ok");
    return 0;
}
