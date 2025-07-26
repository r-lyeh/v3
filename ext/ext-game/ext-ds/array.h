#ifndef ARRAY_H
#define ARRAY_H

#define array_(K)               K*
#define array_back(t)           (array_count(t) > 0 ? (t)+array_count(t)-1 : NULL) // returns pointer to last popped item. NULL if array is empty.
#define array_count(t)          ((int)stbds_arrlen(t)) // count items in array
#define array_erase(t,at)       (array_erasen((t),(at),1)) // erase nth element
#define array_erasefast(t,at)   (stbds_arrdelswap((t),(at))) // swaps Nth element with tail and pops
#define array_erasen(t,at,num)  (stbds_arrdeln((t),(at),(num))) // erase N elements at given index
#define array_insert(t,at,v)    (array_insertn((t),(at),1), (t)[at] = (v), (void)0) // insert item at given index. will shift if needed. new element is zero-clear.
#define array_insertn(t,at,num) (stbds_arrinsn((t),(at),(num)), memset((t)+(at),0,(num)*sizeof(0[t]))) // insert N items at given index. will shift if needed. new elements are zero-clear.
#define array_pop(t)            (array_count(t) ? (stbds_arrpop(t), (t)+array_count(t)) : NULL) // returns pointer to extracted item, or NULL if array is empty
#define array_push(t,v)         (stbds_arrpush((t),(v))) // push item to back
#define array_pushn(t,num)      (stbds_arraddnptr((t),(num))) // push N items to back
#define array_reserve(t,num)    (stbds_arrsetcap((t),(num))) // reserve room for items beforehand
#define array_resize(t,num)     (stbds_arrsetlen((t),(num))) // resize the number of stored items. @todo: check if they're zero-clear
#define array_sort(t, cmpfn)    (qsort((t), array_count(t), sizeof(0[t]), cmpfn))
#define array_clear(t)          (array_resize((t),0)) // clear items in array, but preserve allocated memory for further usage
#define array_free(t)           (stbds_arrfree(t)) // frees the container and sets the pointer to NULL
#define each_array(t,i)         (int i = 0, i##_ = array_count(t); i < i##_; ++i)

#endif
