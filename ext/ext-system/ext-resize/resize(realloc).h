// @todo: allocated infos
// @todo: memset enlarged areas to zero. this is not standard behavior in realloc()

static void* die_if_null(void *ptr) {
    if(!ptr) die("!cannot allocate memory");
    return ptr;
}

void* resize_(void *ptr, size_t sz) {
    if( ptr ) { // either resize, free or info op
        if( !sz ) return free(forget(ptr)), NULL; // free
        if( sz+1 ) return watch(die_if_null(realloc(forget(ptr), sz)),sz); // resize
        return "pointer info"; // info. @todo: size, callstack and/or deleter()
    } else { // either alloc or info op
        if( !sz ) return NULL; // ignore
        if( sz+1 ) return watch(die_if_null(calloc(1, sz)),sz); // alloc
        return "allocator info";
    }
}
