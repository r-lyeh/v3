static int tests, failures;
static void summary(void) {
    if( failures ) fprintf(stderr, "\n%d failures, %d/%d tests passed\n", failures, tests-failures, tests);
}

int test_(int cond, const char *file, int line) {
    ONCE atexit(summary);
    if(!cond) fprintf(stderr,"(F@%s:%d)", file, line);
    return ++tests, failures += !cond, cond;
}
