int    monitors_(void) { return 0; }
double monitor_hz_(int monitor) { return 0; }
double monitor_width_(int monitor) { return 0; }
double monitor_height_(int monitor) { return 0; }
void   monitor_workarea_(int monitor, int rect[4]) { memset(rect, 0, sizeof(int)*4); }
