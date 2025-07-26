// @todo: small things to do in a retail build
// [ ] signal traps
// [ ] app singleton
// [ ] close terminal (win)
// [ ] mount app icon (.ico/.png) close to exe
// [ ] make child windows inherit parent icon (win)

#if V3_RETAIL && V3_MD
#error "using dynamic c runtime in retail build. you'll need to install vc redistributables with your app. are you sure? using /MT would prevent this"
#elif V3_RETAIL && defined _OPENMP
#error "openmp implies dynamic c runtime by default, even in retail builds. you'll need to install vc redistributables with your app. are you sure? removing /openmp would prevent this"
#endif

#if CODE && V3_RETAIL
#pragma once

// ----------------------------------------------------------------------------

#ifndef APP_SINGLETON_GUID
#define APP_SINGLETON_GUID __DATE__ " " __TIME__
#endif

void app_singleton(const char *guid) {
    #ifdef _WIN32
    ONCE {
        char buffer[128];
        snprintf(buffer, 128, "Global\\{%s}", guid);
        static HANDLE app_mutex = 0;
        app_mutex = CreateMutexA(NULL, FALSE, buffer);
        if( ERROR_ALREADY_EXISTS == GetLastError() ) {
            exit(-1);
        }
    }
    #endif
}

// trap signals ---------------------------------------------------------------

#include <signal.h>

const char *trap_name(int signal) {
    if(signal == SIGABRT) return "SIGABRT: abnormal termination";
    if(signal == SIGFPE) return "SIGFPE: floating point exception";
    if(signal == SIGILL) return "SIGILL: illegal instruction";
    if(signal == SIGSEGV) return "SIGSEGV: segmentation fault / invalid memory access";
    if(signal == SIGINT) return "SIGINT: interrupted program";
    if(signal == SIGTERM) return "SIGTERM: termination request";
    ifndef(V3_WINDOWS, if(signal == SIGBUS) return "SIGBUS");
    ifdef(V3_LINUX, if(signal == SIGSTKFLT) return "SIGSTKFLT");
    ifndef(V3_WINDOWS, if(signal == SIGQUIT) return "SIGQUIT");
    return "??";
}
void ignore_trap(int sgn) {
    signal(sgn, ignore_trap);
}
void shutdown_trap(int sgn) {
    signal(sgn, shutdown_trap);
    exit(0);
}
void abort_trap(int sgn) {
    char buffer[1024]; snprintf(buffer, 1024, "Error: unexpected signal %s (%d)\n%s", trap_name(sgn), sgn, trace(+16));
    fprintf(stderr, "%s\n", buffer), alert(buffer), breakpoint();
    signal(sgn, abort_trap);
    exit(-sgn);
}
void debug_trap(int sgn) { // @todo: rename to trap_on_choice() and ask the developer what to do next? abort, continue, debug
    char buffer[1024]; snprintf(buffer, 1024, "Error: unexpected signal %s (%d)\n%s", trap_name(sgn), sgn, trace(+16));
    fprintf(stderr, "%s\n", buffer), alert(buffer), breakpoint();
    signal(sgn, debug_trap);
}
#if V3_WINDOWS
LONG WINAPI seh_trap(PEXCEPTION_POINTERS pExceptionPtrs) {
    char buffer[1024]; snprintf(buffer, 1024, "Error: unexpected SEH exception\n%s", trace(+16));
    fprintf(stderr, "%s\n", buffer), alert(buffer), breakpoint();
    return EXCEPTION_EXECUTE_HANDLER; // Execute default exception handler next
}
#endif

void traps_handled(void) {
    // expected signals
    signal(SIGINT, shutdown_trap);
    signal(SIGTERM, shutdown_trap);
    ifndef(V3_WINDOWS,
    signal(SIGQUIT, shutdown_trap));
    // unexpected signals
    signal(SIGABRT, abort_trap);
    signal(SIGFPE, abort_trap);
    signal(SIGILL, abort_trap);
    signal(SIGSEGV, abort_trap);
    ifndef(V3_WINDOWS,
    signal(SIGBUS, abort_trap));
    ifdef(V3_LINUX,
    signal(SIGSTKFLT, abort_trap));
    // others
    ifdef(V3_WINDOWS,
    SetUnhandledExceptionFilter(seh_trap));
}

// ----------------------------------------------------------------------------

#ifdef _WIN32
// Executables (but not DLLs) exporting thes symbols will use high-performant GPUs on Nvidia Optimus + AMD PowerXpress systems
__declspec(dllexport) DWORD NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif

// ----------------------------------------------------------------------------

AUTORUN {
    ifdef(V3_WINDOWS, FreeConsole()); // detach console
    app_singleton(APP_SINGLETON_GUID); // ensure app cant be run twice (win)
    traps_handled(); // install handlers for diverse traps
    // @todo: -Dmain=WinMain
}

#endif


// @todo: enable this block
// ifdef(V3_RETAIL, AUTORUN {
//     fclose(stderr);
//     fclose(stdout);
//     const char* null_stream = ifdef(V3_WINDOWS, "nul:", "/dev/null");
//     if (!freopen(null_stream, "a", stdout)) die("cannot recreate standard streams");
//     if (!freopen(null_stream, "a", stderr)) die("cannot recreate standard streams");
// } )
