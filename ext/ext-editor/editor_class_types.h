// The editor supports 4 built-in types: bool, int, float, char*
// (more complex types are built from these, e.g., vec2/3/4, mat44...)
//
// There is also a variant `var` type used to homogenize different
// return types in all editing call signatures: `var_t call();`

#ifndef EDITOR_TYPES_H
#define EDITOR_TYPES_H

typedef union var {
    void *pv, *ptr;
    int *pi, i;
    double *pd, d, f;
    char *pc, c, *str;
    bool *pb, b, on;
} var, var_t;

typedef var (*var_fn)();

#define var(...) (ifdef(V3_C,(var),var) { __VA_ARGS__ })
#define var_true var(.b=1)
#define var_false var(.b=0)
#define var_null var(.ptr=0)

#elif CODE
#pragma once

// type[1]
var bool_name() { return var(.str = "bool"); }
var bool_size() { return var(.i = 1); }
AUTORUN {
    editor_add_method("bool", NAME, bool_name);
    editor_add_method("bool", SIZE, bool_size);
}

// type[2]
var short_name() { return var(.str = "short"); }
var short_size() { return var(.i = 2); }
AUTORUN {
    editor_add_method("short", NAME, short_name);
    editor_add_method("short", SIZE, short_size);
}

// type[3]
var string_name() { return var(.str = "string"); }
var string_size() { return var(.i = sizeof(char*)); }
AUTORUN {
    editor_add_method("string", NAME, string_name);
    editor_add_method("string", SIZE, string_size);
}

// type[4]
var float_name() { return var(.str = "float"); }
var float_size() { return var(.i = 4); }
AUTORUN {
    editor_add_method("float", NAME, float_name);
    editor_add_method("float", SIZE, float_size);
}

// type[X]
var vec2_name() { return var(.str = "vec2"); }
var vec2_size() { return var(.i = sizeof(vec2)); }
AUTORUN {
    editor_add_method("vec2", NAME, vec2_name);
    editor_add_method("vec2", SIZE, vec2_size);
}

// type[X]
var vec3_name() { return var(.str = "vec3"); }
var vec3_size() { return var(.i = sizeof(vec3)); }
var vec3_save(vec3 *v) {
    char *buf = va("%*.s", sizeof(vec3), "");
    memcpy(buf, v, sizeof(vec3));
    return var(.ptr = buf);
}
var vec3_load(vec3 *v, void *buf) {
    memcpy(v, buf, sizeof(vec3));
    return var_true;
}
var vec3_edit(vec3 *v) {
    int ret = 0;
    ret |= ui_float("X", &v->x, 0, 100);
    ret |= ui_float("Y", &v->y, 0, 100);
    ret |= ui_float("Z", &v->z, 0, 100);
    return var( .i = ret );
}
#if 0
var vec3_init(vec3*v) { *v = vec3(); return var(.ptr=v); }
var vec3_dump(vec3 *v) { return var(.str = va("(vec3:%f,%f,%f)", v->x,v->y,v->z)); }
var vec3_heap(void *p, size_t l) { return var(.ptr = realloc(p,l)); }
var vec3_iter(vec3 *v, unsigned step) {
    if( step < 3 ) return var( .ptr = (&v->x) + step );
    return var_null;
}
#endif

AUTORUN {
    editor_add_method("vec3", NAME, vec3_name);
    editor_add_method("vec3", SIZE, vec3_size);
    editor_add_method("vec3", SAVE, vec3_save);
    editor_add_method("vec3", LOAD, vec3_load);
    editor_add_method("vec3", EDIT, vec3_edit);

    class_t c = editor_class("vec3");
    assert( 0 == strcmp(c.NAME().str, "vec3"));
    assert( c.SIZE().i == sizeof(vec3) );

    vec3 v = {1,2,3}, z = {0};
    void *buf = c.SAVE(&v).ptr;
    c.LOAD(&z, buf);
    assert(0 == memcmp(&v, &z, sizeof(vec3)));
}

#endif
