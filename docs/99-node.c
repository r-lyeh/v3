#define V3_IMPLEMENTATION
#include "../v3/v3.h"

vec4 add4(vec4 a, vec4 b) { return vec4(.x=a.x+b.x,.y=a.y+b.y,.z=a.z+b.z,.w=a.w+b.w); }

char* va3(vec3 a) { return va("(vec3:%8.3f,%8.3f,%8.3f)", a.x,a.y,a.z); }
char* va4(vec4 a) { return va("(vec4:%8.3f,%8.3f,%8.3f,%8.3f)", a.x,a.y,a.z,a.w); }

typedef struct node_t {
    vec4 pos; // xyz=coords, w=are coords relative to parent? y/n
    struct node_t *parent;
    void *flags; // ECS: rot, sca, objtype; float bbmin[3], bbmax[3], xyradius, radius;
} node_t;

vec4 coords(node_t *n) {
    // if node is relative to parent && has parent...
    if( (((int)n->pos.w)&1) && n->parent ) {
        // ... sum relative pos to parent pos
        vec4 ppos = coords(n->parent);
        return add4(n->pos,ppos);
    }
    // else return absolute pos
    return n->pos;
}

int main() {
    node_t parent = { {1,2,3} };
    node_t child1 = { {10,20,30,0}, &parent };
    node_t child2 = { {-1,-1,-1,1}, &parent };

    puts(va4(coords(&parent)));
    puts(va4(coords(&child1)));
    puts(va4(coords(&child2)));

    return 0;
}
