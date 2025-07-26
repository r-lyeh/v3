#define V3_IMPLEMENTATION
#include "v3.h"

AUTOTEST {
    test( !qintern(NULL,NULL) );  // quark #0, cannot intern null string
    test( !qintern("",NULL) );    // quark #0, ok to intern empty string
    test( !qstring(0,NULL)[0] );  // empty string for quark #0

    unsigned q1 = qintern("Hello",NULL);  // -> quark #1
    unsigned q2 = qintern("happy",NULL);  // -> quark #2
    unsigned q3 = qintern("world.",NULL); // -> quark #3
    printf("%u %u %u\n", q1, q2, q3);

    test( q1 );
    test( q2 );
    test( q3 );
    test( q1 != q2 );
    test( q1 != q3 );
    test( q2 != q3 );

    unsigned q4 = qintern("happy",NULL);
    printf("%x vs %x\n", q2, q4);
    test( q4 );
    test( q4 == q2 );

    char buf[256];
    sprintf(buf, "%s %s %s", qstring(q1,NULL), qstring(q2,NULL), qstring(q3,NULL));
    test( !strcmp("Hello happy world.", buf) );
}
