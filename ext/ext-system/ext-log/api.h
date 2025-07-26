extern int (*lputs)(const char *msg); // [\1..\7] color escape codes. prints callstack if msg[0]=='!'

#if CODE
#pragma once

FILE *stdlog;

#include v3_log_backend

int (*lputs)(const char *) = lputs_;

AUTORUN {
    // to console
    stdlog = stdout;

    // to logfile
    // if( flag("--logfile") )
    // stdlog = fopen(".log.txt","a+");

    // redirect to file
    // freopen(".log.txt", "a+t", stdout);

    #if 0 // << enable if you worried about logging perf
    // Flush automatically every 16 KiB from now
    setvbuf(stdout, NULL, _IOFBF, 16 * 1024);
    #endif

    lputs("\6--- new session ---\7" " " __DATE__ " " __TIME__);
}
#endif
