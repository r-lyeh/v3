#ifndef MAP_H
#define MAP_H

#define map_(K,V)               struct { K key; V value; }
#define map_count(m)            ((int)hmlen(m)) // count items in map
#define map_find(m,k)           (hmgetp_null((m),(k))) // returns pointer to pair. NULL if not found
#define map_insert(m,k,v)       (hmput((m),(k),(v))) // inserts value into map. if already inserted, updates its value
#define map_erase(m,k)          (hmdel((m),(k))) // erases key from map
#define map_free(m)             (hmfree(m)) // frees the container and sets the pointer to NULL
#define each_map(m,i)           (int i = 0, i##_ = map_count(m); i < i##_; ++i) // note: ordered by insertion order. map can be used as pair_t[] array

#define map_sort(m, cmpfn) \
    for( int cnt = map_count(m); cnt; cnt = 0) \
        for( __typeof__(*m) *cpy = (__typeof__(*m) *)memcpy(realloc(0, cnt * sizeof(*m)), m, cnt * sizeof(*m)); cpy; cpy = realloc(cpy, 0) ) \
            for( int i = (qsort(cpy, cnt, sizeof(*m), cmpfn), map_free(m), 0), end = cnt; i < end; ++i) \
                hmputs(m, cpy[i])

// @todo: optimize
#define map_find_or_add(m,k)   (map_find(m,k) ? map_find(m,k) : (map_insert(m,k,0), map_find(m,k)))

#endif
