// @todo: handle base64:// encoded blobs too
// @todo: handle base91:// encoded blobs too

file_t asset_(const char *pathfile) { // file.txt > ./art/dir/file.txt > cook() > read()
    if( strbegi(pathfile, "blob://") ) {
        file_t blob = {0};
        if( sscanf(pathfile+7, "%p:%u", &blob.bin, &blob.len) == 2 ) return blob;
    }
    if( strbegi(pathfile, "https://") || strbegi(pathfile, "http://") || strbegi(pathfile, "ftp://") ) {
        pathfile = url(pathfile);
        if( !pathfile ) return ((file_t){0});
    }
    if( strbegi(pathfile, "file://") ) pathfile += 7;

    // pathfile = lookup(pathfile); // already handled within file_cook
    return file_cook(pathfile);
}
