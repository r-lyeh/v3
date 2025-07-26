void* dll_(const char *fname, const char *symbol) {
#if 0
    if( fname && path_stat(fname) != 1 ) {
        const char *buf, *path = file_path(fname), *base = file_name(fname);
        if( path_stat(buf = va("%s%s.dll", path, base)) == 1 ||
            path_stat(buf = va("%s%s.so", path, base)) == 1 ||
            path_stat(buf = va("%slib%s.so", path, base)) == 1 ||
            path_stat(buf = va("%s%s.dylib", path, base)) == 1 ) {
            fname = (const char *)buf;
        } else {
            return NULL;
        }
    }
#endif
#if V3_WINDOWS
    return (void*)GetProcAddress(fname ? LoadLibraryA(fname) : GetModuleHandleA(NULL), symbol);
#else
    return dlsym(fname ? dlopen(fname, RTLD_NOW|RTLD_LOCAL) : ifdef(V3_MACOS, RTLD_SELF, NULL), symbol);
#endif
}

#if 0 // demo: cl demo.c /LD && REM dll
EXPORT int add2(int a, int b) { return a + b; }
int main() { int (*adder)() = dll("demo.dll", "add2"); printf("%d\n", adder(2,3)); }
#endif
