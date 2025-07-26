uint64_t date_(void) { return 0; }
uint64_t date_epoch_(void) { return 0; }
char *date_string_(void) { static char t[] = "1970-01-01 00:00:00"; return t; }

uint64_t time_ns_(void) { return 0; }
uint64_t time_us_(void) { return 0; }
uint64_t time_ms_(void) { return 0; }
double time_ss_(void) { return 0; }
double time_mm_(void) { return 0; }
double time_hh_(void) { return 0; }

void sleep_ns_( double ns ) {}
void sleep_us_( double us ) {}
void sleep_ms_( double ms ) {}
void sleep_ss_( double ss ) {}

void yield_(void) {}
