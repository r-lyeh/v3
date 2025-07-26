// Note: Windows users add `/Zi` compilation flags, else add `-g` and/or `-ldl` flags
// Note: If you are linking your binary using GNU ld you need to add --export-dynamic

#if V3_MACOS
#include <execinfo.h> // backtrace, backtrace_symbols
#include <dlfcn.h>    // dladdr, Dl_info
#elif V3_GCC && !V3_WASM && !V3_MINGW // maybe V3_LINUX is enough?
#include <execinfo.h>  // backtrace, backtrace_symbols
#elif V3_WINDOWS // && !defined __TINYC__
#include <winsock2.h>  // windows.h alternative
#include <dbghelp.h>
#pragma comment(lib, "DbgHelp")
#pragma comment(lib, "Kernel32")
int backtrace( void **addr, int maxtraces ) {
    static bool init = 0;
    ONCE SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_INCLUDE_32BIT_MODULES);
    ONCE init = SymInitialize(GetCurrentProcess(), NULL, TRUE);
    if(!init) return 0; // error: cannot initialize DbgHelp.lib
#ifdef _MSC_VER // cl and clang-cl
    typedef USHORT (WINAPI *pFN)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG); // _MSC_VER
#else // tcc and mingw
    typedef USHORT (WINAPI *pFN)(); // TINYC
#endif
    static pFN rtlCaptureStackBackTrace = 0;
    if( !rtlCaptureStackBackTrace ) {
        rtlCaptureStackBackTrace = (pFN)GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace");
    }
    if( !rtlCaptureStackBackTrace ) {
        return 0;
    }
    return rtlCaptureStackBackTrace(1, maxtraces, (PVOID *)addr, (DWORD *) 0);
}
char **backtrace_symbols(void *const *list,int size) {
    HANDLE process = GetCurrentProcess();

    struct symbol_t {
        SYMBOL_INFO info;
        TCHAR symbolname[256], terminator;
    } si = { {0} };
    si.info.SizeOfStruct = sizeof(SYMBOL_INFO);
    si.info.MaxNameLen = sizeof(si.symbolname) / sizeof(TCHAR); // number of chars, not bytes

    IMAGEHLP_LINE l64 = { 0 };
    l64.SizeOfStruct = sizeof(IMAGEHLP_LINE);

    static threadlocal_ char **symbols = 0; //[32][64] = {0};
    if( !symbols ) {
        symbols = realloc(0, 128 * sizeof(char*)); // do not use REALLOC
        for( int i = 0; i < 128; ++i) symbols[i] = realloc(0, 128 * sizeof(char)); // do not use REALLOC
    }

    if(size > 128) size = 128;
    for( int i = 0; i < size; ++i ) {

        char *ptr = symbols[i];
        *ptr = '\0';

        if (SymFromAddr(process, (DWORD64)(uintptr_t)list[i], 0, &si.info)) {
            //char undecorated[1024];
            //UnDecorateSymbolName(si.info.Name, undecorated, sizeof(undecorated)-1, UNDNAME_COMPLETE);
            char* undecorated = (char*)si.info.Name;
            ptr += snprintf(ptr, 128, "%s", undecorated);
        } else {
            ptr += snprintf(ptr, 128, "%s", "(?""?)");
        }

        DWORD dw = 0;
        if (SymGetLineFromAddr(process, (DWORD64)(uintptr_t)list[i], &dw, &l64)) {
            ptr += snprintf(ptr, 128 - (ptr - symbols[i]), " (%s:%u)", l64.FileName, (unsigned)l64.LineNumber);
        }
    }

    return symbols;
}
#else
int backtrace(void **heap, int num) { return 0; }
char **backtrace_symbols(void *const *sym,int num) { return 0; }
#endif

char *trace_( int traces ) {
#if V3_TCC && V3_LINUX
    return "";
#endif
#if V3_WASM // there is a stack overflow failure somewhere in the impl below
    static char empty[1]; return empty[0] = '\0', empty;
#endif

    enum { skip = 1 }; /* exclude 1 trace from stack (this function) */
    enum { maxtraces = 96 };

    static threadlocal_ char *output = 0;
    if(!output ) output = realloc( 0, maxtraces * (128+2) ); // do not use REALLOC
    if( output ) output[0] = '\0';
    char *ptr = output;

    int inc = 1;
    if( traces < 0 ) traces = -traces, inc = -1;
    if( traces == 0 ) return "";
    if( traces > maxtraces ) traces = maxtraces;

    void* stacks[maxtraces + 1]; stacks[maxtraces] = NULL; // = { 0 };
    traces = backtrace( stacks, traces );
    char **symbols = backtrace_symbols( stacks, traces ); // @todo: optimization: map(void*,char*) cache; and retrieve only symbols not in cache

    char demangled[1024] = "??";
    int L = 0, B = inc>0 ? skip - 1 : traces, E = inc>0 ? traces : skip - 1;
    for( int i = B; ( i += inc ) != E; ) {
#if V3_LINUX
        #if ENABLE_LINUX_CALLSTACKS
        // @fixme: following snippet works if compiled with '-g', albeit terribly slow
        // should concat addresses into a multi-address line

        char *binary = symbols[i];
        char *address = strchr( symbols[i], '(' ) + 1;
        *strrchr( address, ')') = '\0'; *(address - 1) = '\0';

        char addr2line[128]; snprintf(addr2line, 128, "addr2line -e %s %s", binary, address);

        for( FILE *fp = popen(addr2line, "r"); fp ; pclose(fp), fp = 0 ) { //addr2line -e binary -f -C address
            fgets(demangled, sizeof(demangled), fp);
            int len = strlen(demangled); while( len > 0 && demangled[len-1] < 32 ) demangled[--len] = 0;
        }
        symbols[i] = demangled;
        #else
        // make it shorter. ie, `0x00558997ccc87e ./a.out(+0x20187e) [0x00558997ccc87e]`
        strchr(symbols[i], ')')[1] = '\0';
        #endif
#elif V3_MACOS
        /*struct*/ Dl_info info;
        if( dladdr(stacks[i], &info) && info.dli_sname ) {
            const char *dmgbuf = info.dli_sname[0] != '_' ? NULL :
                 ifdef(V3_CPP, __cxa_demangle(info.dli_sname, NULL, 0, NULL), info.dli_sname);
            strcpy( demangled, dmgbuf ? dmgbuf : info.dli_sname );
            symbols[i] = demangled;
            ifdef(V3_CPP, dmgbuf && free( (void*)dmgbuf ) );
        }
#endif
        if( symbols[i] )
        ptr += sprintf(ptr, "%03d: %p %s\n", ++L, (void*)(uintptr_t)stacks[i], symbols[i]); // format gymnastics because %p is not standard when printing pointers
    }

#if V3_LINUX || V3_MACOS
     if(symbols) free(symbols);
#endif

     return output ? output : "";
}

char* tracef_( FILE *fp, int traces ) {
    char *buf = trace(traces);
    fputs(buf, fp);
    fflush(fp);
    return buf;
}
