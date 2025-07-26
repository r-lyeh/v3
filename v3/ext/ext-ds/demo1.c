#define V3_IMPLEMENTATION
#include "../../v3.h"

void quarks_test() {
    test( !quark_intern(NULL) ); // quark #0, cannot intern null string
    test( !quark_intern("") );   // quark #0, ok to intern empty string
    test( !quark_string(0)[0] );  // empty string for quark #0

    unsigned q1 = quark_intern("Hello");  // -> quark #1
    unsigned q2 = quark_intern("happy");  // -> quark #2
    unsigned q3 = quark_intern("world."); // -> quark #3
    printf("%u %u %u\n", q1, q2, q3);

    test( q1 );
    test( q2 );
    test( q3 );
    test( q1 != q2 );
    test( q1 != q3 );
    test( q2 != q3 );

    unsigned q4 = quark_intern("happy");
    printf("%x vs %x\n", q2, q4);
    test( q4 );
    test( q4 == q2 );

    char buf[256];
    sprintf(buf, "%s %s %s", quark_string(q1), quark_string(q2), quark_string(q3));
    test( !strcmp("Hello happy world.", buf) );
}

int main() {
    quarks_test();
    return 0;
}
