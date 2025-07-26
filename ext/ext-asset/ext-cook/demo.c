#define V3_IMPLEMENTATION
#include "v3.h"

// demo.c (cook)
int main() {
    double then = time_ss();

    unsigned zlevel = flagi("--level,-l", 1); // [0..15]
    unsigned zflags = flagi("--compressor,-c", 0/*COMPRESS_ULZ*/); // 0 for deflate

    // zipfile can be null: --zipfile= 
    // if zipfile is null, no zipfile will be written.
    const char* zipfile = flags("--zipfile,-z", ".cook.zip");
    zip_t* z = zipfile ? zip_open(zipfile, "wb") : NULL;

    int numfiles;
    char **list = lsr("./", &numfiles);

    int count = numfiles;
    while( count-- ) {
        // skip folders in list
        if( strchr("\\/", list[count][strlen(list[count])-1]) ) continue;

        // skip cook process if file is already cooked
        if( path_name(list[count])[0] == '.' ) continue;

        // allow art/data/demo/asset folders. disallow otherwise
        if( strstri(list[count], "/art/")
            || strstri(list[count], "/artwork/")
            || strstri(list[count], "/data/")
            || strstri(list[count], "/datas/")
            || strstri(list[count], "/demo/")
            || strstri(list[count], "/demos/")
            || strstri(list[count], "/asset/")
            || strstri(list[count], "/assets/")
        ) { /* ok */ } else continue;

        // exclude emdeb/* files. they are already handled somewhere else
        if( strstri(list[count], "/embed/")) continue;

        printf("Cooking %s...\n", list[count]);

        // cook & read
        file_t f = file_cook(list[count]);

        // append cooked file to zip, if needed
        if( z ) {
            zip_append_mem(z, list[count], ""/*comment*/, f.bin, f.len, zflags | zlevel);
        }
    }

    if(z) zip_close(z);

    printf("%5.2fs, %d files\n", time_ss() - then, numfiles);
    return 0;
}
