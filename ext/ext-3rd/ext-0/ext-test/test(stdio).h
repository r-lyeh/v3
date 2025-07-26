static int tests, errors;
static void summary(void) {
    fprintf(stderr, "\n%d failures, %d/%d tests passed\n", errors, tests-errors, tests);
}
int test_(int cond, const char *file, int line) {
    ONCE atexit(summary);
    if(!cond) fprintf(stderr,"(F@%s:%d)", file, line);
    return ++tests, errors += !cond, cond;
}
int failures_(void) {
    return errors;
}
