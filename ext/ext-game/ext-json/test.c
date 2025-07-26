AUTOTEST {
    const char json5[] =
    "  /* json5 */ // comment\n"
    "  abc: 42.67, def: true, integer:0x100 \n"
    "  huge: 2.2239333e5, \n"
    "  hello: 'world /*comment in string*/ //again', \n"
    "  children : { a: 1, b: 2, c: 3 },\n"
    "  array: [+1,2,-3,4,5],    \n"
    "  invalids : [ nan, NaN, -nan, -NaN, inf, Infinity, -inf, -Infinity ],";
    if( json_push(json5) ) {
        test( json_float("/abc") == 42.67 );
        test( json_int("/def") == 1 );
        test( json_int("/integer") == 0x100 );
        test( json_float("/huge") > 2.22e5 );
        test( strlen(json_string("/hello")) == 35 );
        test( json_int("/children/a") == 1 );
        test( json_int("/children.b") == 2 );
        test( json_int("/children[c]") == 3 );
        test( json_int(va("/array[%d]", 2)) == -3 );
        test( json_count("/invalids") == 8 );
        test( isnan(json_float("/invalids[0]")) );
        test( !json_node("/non_existing") );
        json_pop();
    }
}
