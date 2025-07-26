
#if !CODE



API bool data_tests();

#else

// ----------------------------------------------------------------------------

bool data_tests() {
    // data tests (json5)
    const char json5[] =
    "  /* json5 */ // comment\n"
    "  abc: 42.67, def: true, integer:0x100 \n"
    "  huge: 2.2239333e5, \n"
    "  hello: 'world /*comment in string*/ //again', \n"
    "  children : { a: 1, b: 2, c: 3 },\n"
    "  array: [+1,2,-3,4,5],    \n"
    "  invalids : [ nan, NaN, -nan, -NaN, inf, Infinity, -inf, -Infinity ],";
    if( json_push(json5) ) {
        assert( json_float("/abc") == 42.67 );
        assert( json_int("/def") == 1 );
        assert( json_int("/integer") == 0x100 );
        assert( json_float("/huge") > 2.22e5 );
        assert( strlen(json_string("/hello")) == 35 );
        assert( json_int("/children/a") == 1 );
        assert( json_int("/children.b") == 2 );
        assert( json_int("/children[c]") == 3 );
        assert( json_int("/array[%d]", 2) == -3 );
        assert( json_count("/invalids") == 8 );
        assert( isnan(json_float("/invalids[0]")) );
        assert( !json_find("/non_existing") );
        assert( PRINTF("json5 tests OK\n") );
        json_pop();
    }

    return true;
}

#endif
