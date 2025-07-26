void* script_create_() { return NULL; }
void script_destroy_(void *L) {}
bool script_runfile_(void *L, const char *fname) { return 0; }
bool script_runstring_(void *L, const char *str) { return 0; }
bool script_preload_(void *L, const char *modname, const char *modcode) { return 0; }
