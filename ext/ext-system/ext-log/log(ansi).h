int lputs_(const char *msg) {
    double clk = time_ss();
    const char *ansi[] = {
        [1] = "\033[34m", [2] = "\033[31m", [3] = "\033[35m",
        [4] = "\033[32m", [5] = "\033[36m", [6] = "\033[33m", [7] = "\033[0m"
    };
    FILE *out = stdlog ? stdlog : stdout;
    flockfile_(out);

        if(msg[0] == '!') tracef(out, +16), ++msg; // @todo

        static threadlocal_ int _thread_id;
        fprintf(out,"%03d %06.2f ", (((unsigned)(uintptr_t)&_thread_id)>>8) % 1000, clk);
        // could be also: time_t t = time(0); fprintf(out,"%.8s",11+ctime(&t));

        while( *msg ) *msg > 0 && *msg <= 7 ? fprintf(out,"%s", ansi[*msg++]) : fputc(*msg++, out);
        fprintf(out, "%s\n", ansi[7]);

    funlockfile_(out);
    return 1;
}

#ifdef _WIN32
static void colors_off(void) {
    printf("\033[0m");
}
AUTORUN {
    // enable colors
    DWORD mode = 0; SetConsoleMode(GetStdHandle(-11), (GetConsoleMode(GetStdHandle(-11), &mode), mode|4)); // enable colors
    atexit(colors_off);
    // enable unicode within terminal ()
    WinExec("chcp 65001 >nul", SW_HIDE); // note: cannot invoke _popen()/system() from dll, hence WinExec()
}
#endif
