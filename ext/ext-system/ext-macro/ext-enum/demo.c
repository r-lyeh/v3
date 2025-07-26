#define V3_IMPLEMENTATION
#include "v3.h"

TYPEDEF_ENUM( FRUIT, apple, orange, grape = 10 ); // @todo: support `banana = grape + 2`

int main() {
    printf("%s=%d(%d)\n", FRUITstr(apple),  FRUITint("apple"), apple);
    printf("%s=%d(%d)\n", FRUITstr(orange), FRUITint("orange"), orange);
    printf("%s=%d(%d)\n", FRUITstr(grape),  FRUITint("grape"), grape);
    printf("%d\n", FRUITint("orange|grape"));
    return 0;
}

// apple=0(0)
// orange=1(1)
// grape=10(10)
// 11
