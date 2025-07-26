AUTOTEST {
    test( eval("1+1") == 2 );         // common path
    test( eval("1+") != eval("1+") ); // check that errors return NAN
}
