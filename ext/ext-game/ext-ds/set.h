#ifndef SET_H
#define SET_H

#define set_(K)                 map_(K,int)
#define set_count(m)            map_count(m)
#define set_find(m,k)           map_find(m,k)
#define set_insert(m,k)         map_insert(m,k,0)
#define set_erase(m,k)          map_erase(m,k)
#define set_free(m)             map_free(m)
#define each_set(m,i)           map_each(m,i)

#define set_sort(m,cmpfn)       map_sort(m,cmpfn)

#endif
