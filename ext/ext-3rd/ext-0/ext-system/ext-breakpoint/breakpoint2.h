#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
// For potential future use of /proc/self/status
#include <string.h>
#endif

// Flag to indicate if SIGTRAP handler was called
static volatile sig_atomic_t sigtrap_handled = 0;

// Signal handler for SIGTRAP
static void _sigtrap_handler(int signum, siginfo_t *info, void *context) {
    sigtrap_handled = 1; // Mark that handler was called (no debugger)
    // Do not restore SIG_DFL here to avoid termination
}

// Check if a debugger is present (Linux/macOS)
static int has_debugger(void) {
    struct sigaction sa, oldsa;
    sigtrap_handled = 0; // Reset flag before check

    // Set up SIGTRAP handler
    sa.sa_sigaction = _sigtrap_handler;
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND; // Reset handler after first signal
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGTRAP, &sa, &oldsa) == -1) {
        perror("sigaction failed");
        return 0; // Fail safely (assume no debugger)
    }

    // Raise SIGTRAP
    raise(SIGTRAP);

    // If sigtrap_handled is 1, handler was called (no debugger)
    // If sigtrap_handled is 0, debugger likely intercepted SIGTRAP
    int result = !sigtrap_handled;

    // Restore original SIGTRAP handler
    if (sigaction(SIGTRAP, &oldsa, NULL) == -1) {
        perror("sigaction restore failed");
    }

    return result;

    // Alternative Linux-specific check (commented out for future use)
    /*
    #ifdef __linux__
        FILE *fp = fopen("/proc/self/status", "r");
        if (fp) {
            char line[256];
            int tracer_pid = 0;
            while (fgets(line, sizeof(line), fp)) {
                if (strncmp(line, "TracerPid:", 10) == 0) {
                    tracer_pid = atoi(line + 10);
                    break;
                }
            }
            fclose(fp);
            return tracer_pid != 0;
        }
    #endif
    */
}

// Trigger a breakpoint (pauses if debugger is present)
static void breakpoint(void) {
    if (has_debugger()) {
        raise(SIGTRAP); // Trigger SIGTRAP for debugger to catch
        // Note: No handler is set here, so debugger will pause
        // Alternative: __builtin_trap(); // Compiler-specific trap
        // Alternative: __asm__ volatile("int $0x03"); // x86-specific breakpoint
    }
}

#endif // DEBUGGER_H
