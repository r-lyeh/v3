int lprintf_(const char *msg) {
    int errno_ = errno;
    double clk = time_ss();
    const char *ansi[] = {
        [1] = "\033[34m", [2] = "\033[31m", [3] = "\033[35m",
        [4] = "\033[32m", [5] = "\033[36m", [6] = "\033[33m", [7] = "\033[0m"
    };
    FLOCKFILE(stdlog);

        if(msg[0] == '!') tracef(stdlog, +16), ++msg; // @todo

        static int THREAD _thread_id;
        fprintf(stdlog,"%03d %06.2f ", (((unsigned)(uintptr_t)&_thread_id)>>8) % 1000, clk);

        // could be also: time_t t = time(0); fprintf(stdlog,"%.8s",11+ctime(&t));

        while( *msg ) *msg > 0 && *msg <= 7 ? fprintf(stdlog,"%s", ansi[*msg++]) : fputc(*msg++, stdlog);
        if(errno_) fprintf(stdlog," %s%s%s\n", ansi[2], strerror(errno_), ansi[7]); else fprintf(stdlog, "%s\n", ansi[7]);

    FUNLOCKFILE(stdlog);
    return 1;
}

#ifdef _WIN32
static void colors_off(void) {
    printf("\033[0m");
}
AUTORUN {
    DWORD mode = 0; SetConsoleMode(GetStdHandle(-11), (GetConsoleMode(GetStdHandle(-11), &mode), mode|4)); // enable colors
    _pclose(_popen("chcp 65001 >nul", "r")); // enable unicode within terminal
    atexit(colors_off);
}
#endif

// @todo: enable this block
// ifdef(retail, AUTORUN {
//     fclose(stderr);
//     fclose(stdout);
//     const char* null_stream = ifdef(win32, "nul:", "/dev/null");
//     if (!freopen(null_stream, "a", stdout)) die("cannot recreate standard streams");
//     if (!freopen(null_stream, "a", stderr)) die("cannot recreate standard streams");
// } )
