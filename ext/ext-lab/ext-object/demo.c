#define V3_IMPLEMENTATION
#include "v3.h"

typedef struct Vec2 {
    void *vt;
    int x,y;
} Vec2;

extern void (*Vec2_vt[])();

void Vec2_type(const char **out) { *out = "Vec2"; }
void Vec2_echo(Vec2 *v, const char **out) { *out = va("%d,%d", v->x,v->y); }

void (*Vec2_vt[92])() = {
    ['T']=Vec2_type,
    ['E']=Vec2_echo,
};

typedef struct Vec3 {
    void *vt;
    int x,y,z;
    int *other;
} Vec3;

extern void (*Vec3_vt[])();

void Vec3_type(const char **out) { *out = "Vec3"; }
void Vec3_echo(Vec3 *v, const char **out) { *out = va("%d,%d,%d", v->x,v->y,v->z); }
void Vec3_drop(Vec3 *v) { if(v->other) free(v->other); }

void (*Vec3_vt[92])() = {
    ['T']=Vec3_type,
    ['E']=Vec3_echo,
    ['D']=Vec3_drop,
};

int main() {
    Vec2 ab = make(Vec2,1,2);
    Vec2 *cd = heap(Vec2,3,4);
    Vec3 abc = make(Vec3,1,.z=2);
    Vec3 *def = heap(Vec3);
    Vec3 *ghi = heap(Vec3,.z=1,.y=2,.x=3);

    puts(echo(&ab));
    puts(echo(cd));
    puts(echo(&abc));
    puts(echo(def));
    puts(echo(ghi));

    assert(same(&ab,cd));
    assert(same(&abc,def));
    assert(!same(cd,def));
    assert(~puts("Ok"));

    drop(cd);
    drop(def);
    drop(ghi);

    return 0;
}

// Vec2:1,2
// Vec2:3,4
// Vec3:1,0,2
// Vec3:0,0,0
// Vec3:3,2,1
