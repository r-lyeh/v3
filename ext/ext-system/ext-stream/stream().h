typedef map_(uint64_t, array_(char))* streams_t;
static threadlocal_ streams_t streams;

static
streams_t stream_find_or_create_(uint64_t id) {
    streams_t found = map_find(streams, id);
    if( !found ) { map_insert(streams, id, NULL); found = map_find(streams, id); }
    return found;
}
void  stream_drop_(uint64_t id) {
    streams_t found = stream_find_or_create_(id);
    array_free(found->value), found->value = NULL;
}
int   stream_size_(uint64_t id) {
    streams_t found = stream_find_or_create_(id);
    return array_count(found->value);
}
int   stream_grow_(uint64_t id, int pos) {
    streams_t found = stream_find_or_create_(id);
    if( pos < 0 ) pos = 0;
    return array_resize(found->value, pos), pos;
}
void* stream_data_(uint64_t id) {
    streams_t found = stream_find_or_create_(id);
    return found->value;
}
int   stream_push_(uint64_t id, const void *ptr, int len) {
    streams_t found = stream_find_or_create_(id);
    int tell = array_count(found->value);
    array_resize(found->value, tell + len);
    memcpy(found->value+tell, ptr, len);
    return tell;
}

#if 0 // hexdumps all items in a factory. assumes ids are made of factory:32|item:32
void stream_dump_(unsigned factory) { // prints all bits in a category
    uint64_t fac64 = (factory * 1ull) << 32;
    uint64_t mask = (~0u * 1ull) << 32;
    for( int i = 0, e = map_count(streams); i < e; ++i ) {
        if( (streams[i].key & mask) == fac64 )
            printf("%u %u ", (int)(streams[i].key >> 32), (int)(streams[i].key & ~0u)),
            hexdump( streams[i].value, clamp(array_count(streams[i].value), 0, 64) );
    }
}
#endif
