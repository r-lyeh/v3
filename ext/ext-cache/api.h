extern void *(*cache)(unsigned factory, unsigned key, void *value); // insert or refresh item into cache factory.
extern void *(*find)(unsigned factory, unsigned key); // retrieve item from cache factory.

#if CODE
#pragma once

typedef map_(uint64_t, void *) cache_t;

cache_t cached_;

void *cache_(unsigned factory, unsigned key, void *value ) {
    uint64_t key64 = (factory * 1ull) << 32 | key;
    cache_t entry = map_insert(cached_, key64, value);
    return value;
}
void* find_(unsigned factory, unsigned key ) {
    uint64_t key64 = (factory * 1ull) << 32 | key;
    cache_t entry = map_find(cached_, key64);
    return entry ? entry->value : NULL;
}
void list(unsigned factory, const char *(*stringify)(void *)) { // prints all cached items in a category
    uint64_t fac64 = (factory * 1ull) << 32;
    uint64_t mask = (~0u * 1ull) << 32;
    for( int i = 0, e = map_count(cached_); i < e; ++i ) {
        if( (cached_[i].key & mask) == fac64 )
        printf("%u %u %s\n", (int)(cached_[i].key >> 32), (int)(cached_[i].key & ~0u), !cached_[i].value ? "" : ( stringify ? stringify(cached_[i].value) : (const char *)cached_[i].value ));
    }
}

void *(*cache)(unsigned factory, unsigned key, void *value) = cache_;
void *(*find)(unsigned factory, unsigned key) = find_;

#endif
