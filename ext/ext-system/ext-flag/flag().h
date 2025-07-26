const char *flag_(const char *opt) {
    for( int i = 1; i < argc(); ++i ) {
        if( argv(i)[0] != '-' ) continue;

        const char *found = strbegi(argv(i), opt);
        if( found ) {
            found += strlen(opt);
            if( *found == '=' ) return found + 1;
            if( *found == '\0' ) return "";
        }
    }
    return NULL;
}
int flagi_(const char *opt, int defaults) {
    const char *rc = flag(opt);
    return rc && rc[0] ? atoi(rc) : defaults;
}
float flagf_(const char *opt, float defaults) {
    const char *rc = flag(opt);
    return rc && rc[0] ? atof(rc) : defaults;
}
const char* flags_(const char *opt, const char *defaults) {
    const char *rc = flag(opt);
    return rc && rc[0] ? rc : defaults;
}
