#define V3_IMPLEMENTATION
#include "v3.h"

void ds1() {
    map_(int,char) *foo = 0,swap; assert(!foo);
                            assert(map_count(foo) == 0);

    map_insert(foo,3,'c');  assert(map_find(foo,3)->value == 'c');
    map_insert(foo,1,'a');  assert(map_find(foo,1)->value == 'a'); assert(foo);
    map_insert(foo,2,'b');  assert(map_find(foo,2)->value == 'b');
                            assert(!map_find(foo,0));

    for( int i = 0; i < map_count(foo); ++i )
        printf("%d %c\n", foo[i].key, foo[i].value);

    map_erase(foo,2);       assert(!map_find(foo,2));
    map_insert(foo,2,'d');  assert(map_find(foo,2)->value == 'd');
                            assert(!map_find(foo,0));

    for( int i = 0; i < map_count(foo); ++i )
        printf("%d %c\n", foo[i].key, foo[i].value);

    map_free(foo);          assert(!foo);

    assert(~puts("Ok\n---"));
}

void ds2() {
    map_(float, char) *h = 0;

    map_insert(h, 10.5, 'h');
    map_insert(h, 20.4, 'e');
    map_insert(h, 50.3, 'l');
    map_insert(h, 40.6, 'X');
    map_insert(h, 30.9, 'o');

    printf("%c - ", map_find(h, 40.6)->value);

    map_insert(h, 40.6, 'l');

    for each_map(h, i)
        printf("%c ", h[i].value);

    assert(~puts("\nOk\n---"));
}

void ds3() {
    int t;
    array_(int) v = 0;       assert(array_count(v) == 0);
    array_pop(v);            assert(array_count(v) == 0);
    array_reserve(v,4);      assert(array_count(v) == 0);
    array_resize(v,4);       assert(array_count(v) == 4);
    array_resize(v,0);       assert(array_count(v) == 0);
    array_push(v,0);         assert(array_count(v) == 1 && v[0] == 0); 
    array_push(v,1);         assert(array_count(v) == 2 && v[1] == 1);
    array_push(v,2);         assert(array_count(v) == 3 && v[2] == 2);
    for each_array(v,i)      printf("%d\n", v[i]); puts("---");
t= *array_pop(v);            assert(array_count(v) == 2 && t == 2);
t= *array_pop(v);            assert(array_count(v) == 1 && t == 1);
t= *array_pop(v);            assert(array_count(v) == 0 && t == 0);
    array_pop(v);            assert(array_count(v) == 0);
    array_free(v);           assert(array_count(v) == 0);
    array_push(v,1);         assert(array_count(v) == 1);
    array_push(v,2);         assert(array_count(v) == 2);
    array_push(v,3);         assert(array_count(v) == 3);
    array_push(v,4);         assert(array_count(v) == 4);
    array_insertn(v,2,3);    assert(array_count(v) == 7 && !v[2] && !v[3] && !v[4]);
    for each_array(v,i)      printf("%d\n", v[i]); puts("---");
    array_clear(v);          assert(array_count(v) == 0);
    array_pop(v);            assert(array_count(v) == 0);
    array_free(v);           assert(array_count(v) == 0);
    assert(~puts("Ok\n---"));
}

int main() {
    ds1();
    ds2();
    ds3();
    return 0;
}
