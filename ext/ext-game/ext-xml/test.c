AUTOTEST {
    const char *xml =
    "<!-- XML representation of a person record -->"
    "<person created=\"2006-11-11T19:23\" modified=\"2006-12-31T23:59\">"
    "    <firstName>Robert</firstName>"
    "    <lastName>Smith</lastName>"
    "    <address type=\"home\">"
    "        <street>12345 Sixth Ave</street>"
    "        <city>Anytown</city>"
    "        <state>CA</state>"
    "        <postalCode>98765-4321</postalCode>"
    "    </address>"
    "</person>";
    if( xml_push(xml) ) {
        test( strcmp("Robert", xml_string("/person/firstName/$")) == 0 );
        test( strcmp("Smith", xml_string("/person/lastName/$")) == 0 );
        test( strcmp("home", xml_string("/person/address/@type")) == 0 );
        xml_pop();
    }
}
