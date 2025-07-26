void* app_create_(const char *title, const char *flags) { errno = 0; return NULL; }
void   app_icon_(const char *iconfile) {}
void*  app_handle_(void) { return 0; }
void*  app_handle_native_(void) { return 0; }
int    app_width_(void) { return 0; }
int    app_height_(void) { return 0; }
char*  app_flags_(void) { static char e[1]; return e[0] = '\0', e; }
float  app_framerate(void) { return 60; }
int    app_swap_(void) { return 0; }
void   app_show_(bool on) {}
void*  app_proc_(const char *name) { return NULL; }
void  app_destroy_(void) {}
