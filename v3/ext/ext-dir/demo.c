#define V3_IMPLEMENTATION
#include "../../v3.h"

int main() {
    int count;
    char **list = ls(".", &count);
    test( list );
    test( count > 0 );
    test( list[0] );
    test( list[count] == 0 );
    for( int i = 0; i < count; ++i ) puts(list[i]);
    puts("Ok");
    return 0;
}
