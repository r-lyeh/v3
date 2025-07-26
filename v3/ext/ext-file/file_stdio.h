#define file_read_ file_read2_
#define file_save_ file_save2_
#define file_size_ file_size2_
#include "file_null.h"
#undef  file_size_
#undef  file_save_
#undef  file_read_

file_t file_read_(const char *fname) {
    file_t b = {0};
    for( FILE *fp = fopen8(fname, "rb"); fp; fclose(fp), fp = 0 ) {
        strncpy(b.url, fname, COUNTOF(b.url));

        fseek(fp, 0L, SEEK_END);
        b.len = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        static THREAD char* slots[FILE_MAX_SLOTS] = {0};
        static THREAD int idx = 0; idx = (idx + 1) % COUNTOF(slots);
        b.bin = (slots[idx] = realloc(slots[idx], b.len+1));
        b.len[(char*)b.bin] = '\0';

        if( fread(b.bin, b.len, 1, fp) ) return fclose(fp), b;
    }
    return file_read2_(fname);
}
bool file_save_(const char *pathfile, const void *blob, unsigned len) {
    FILE *fp = fopen8(pathfile, "wb");
    if( !fp ) return 0;
    bool ok = fwrite(blob, len, 1, fp) == 1;
    return fclose(fp), ok;
}
unsigned file_size_(const char *pathfile) {
    struct stat8_t st;
    return stat8(pathfile, &st) >= 0 ? (unsigned)st.st_size : 0u;
}
