#ifndef FILE_MAX_SLOTS
#define FILE_MAX_SLOTS 16
#endif

file_t blob_(void *bin, unsigned len) {
    return ((file_t){bin, len, ifndef(retail, {0})});
}

file_t file_read_(const char *fname) {
    if( strbegi(fname, "blob://") ) {
        file_t blob = {0};
        if( sscanf(fname+7, "%p,%u", &blob.bin, &blob.len) == 2 ) return blob;
    }
    return blob_(NULL, 0);
}
bool file_save_(const char *pathfile, const void *blob, unsigned len) {
    return false;
}
unsigned file_size_(const char *pathfile) {
    return 0;
}
