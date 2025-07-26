// generate a memory leak file report (.memleaks.XXX.txt) and print it to console on app exit

static char generation[4]; // helps at identifying concurrent apps running on very same folder

void* watch_(void* ptr, size_t sz) {
    static threadlocal_ int open = 1;
    if( open ) {
        open = 0;

        char fname[64]; snprintf(fname, sizeof(fname), ".%p.%s", ptr, generation);
        for(FILE *fp = fopen(fname, "wb"); fp; fclose(fp), fp = 0) {
            fprintf(fp, "%08u bytes leaked, callstack: %s\n", (unsigned)sz, replace(trace(16), "\n", " "));
        }

        open = 1;
    }
    return ptr;
}
void* forget_(void* ptr) {
    char fname[64]; snprintf(fname, sizeof(fname), ".%p.%s", ptr, generation);
    unlink_(fname);
    return ptr;
}
static void leak_summary(void) { // sorted by descending size
    char script[512] =
    ifdef(V3_WINDOWS,
        "if exist .*.XXX ((type .*.XXX | sort /r) > .memleaks.XXX.txt 2>nul) & "
        "del .*.XXX >nul 2>nul & "
        "if exist .memleaks.XXX.txt for /f %i in ('find /c /v \"\" ^< .memleaks.XXX.txt') do @if %i gtr 0 echo %i memleaks found; details in .memleaks.XXX.txt file.";
        ,
        "(cat .*.XXX | sort -r) > .memleaks.XXX.txt 2>/dev/null ; "
        "rm .*.XXX 2>/dev/null ; "
        "if [ -s .memleaks.XXX.txt ]; then echo \"$(wc -l < .memleaks.XXX.txt) memleaks found; details in .memleaks.XXX.txt file.\"; fi";
    );
    system(replace(script, "XXX", generation));
}
AUTORUN {
    static threadlocal_ char genid;
    uint64_t hash = (uint64_t)(uintptr_t)&genid;
    snprintf(generation, sizeof(generation), "%03u", (int)(fnv1a(&hash, sizeof(hash) % 1000)));
    atexit(leak_summary);
}
