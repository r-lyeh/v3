// The place where types and actions are linked together is the classmatrix (actions x types).
// Roughly speaking, this is a large .csv file, where columns are actions and rows are types. Therefore,
// - Define new editing operations by inserting new columns in the classmatrix.
// - Define new game types by inserting new rows in the classmatrix.
//
// [editor.classes]
// name,size,load,save,init,tick,draw,... ; header
// bool_name,bool_size,bool_load,bool_save,,,, ... ; built-in type
// float_name,float_size,float_load,float_save,,,, ... ; built-in type
// vec3_name,vec3_size,vec3_load,vec3_save,vec3_init,,, ... ; built-in type
// player_name,... ; custom type
// enemy1_name,... ; custom type
//
// For convenience, you can also access each row type by using a generic `class_t` wrapper struct.

#ifndef EDITOR_CLASS_H
#define EDITOR_CLASS_H

// setter

int     editor_add_method(const char *classname, int method, void *func, const char *func_str);

// getter

#pragma pack(push, 1)
typedef struct class_t {
    #define DECLARE_FUNCTOR(x) var_t (*x)();
    FOREACH_TYPE_ACTION(DECLARE_FUNCTOR)
    #undef DECLARE_FUNCTOR
} class_t;
#pragma pack(pop)

class_t editor_class(const char *classname);

// utils

#define editor_add_method(c,m,fn) editor_add_method(c,m,fn,#fn)
void    editor_dump_class(FILE *out, const char *mask);

#elif CODE
#pragma once

static array_(void*) matrix;     // array of [PROPS(COLS >)*TYPES(ROWS v)]
static array_(const char*) symb; // array of [PROPS(COLS >)*TYPES(ROWS v)]

static int editor_push_row(void *properties_table) {
    // ensure first row is actually the name of each column
    static int once = 0; if(!once) { once = 1;
        char* property_names[] = {
            #define DEFINE_LITERAL(x) #x,
            FOREACH_TYPE_ACTION(DEFINE_LITERAL)
            #undef DEFINE_LITERAL
        };
        editor_push_row(property_names);
    }

    int type = array_count(matrix);
    array_resize(matrix, type + PROPS);
    array_resize(symb,   type + PROPS);
    if( properties_table ) memcpy(matrix + type, properties_table, sizeof(void*) * PROPS);
    return type / PROPS;
}
static unsigned editor_find_row(const char *nametype) {
    for( int i = PROPS, e = array_count(matrix); i < e; i += PROPS ) {
        var_t (*fn)() = matrix[i+NAME];
        if( fn && !strcmp(fn().str, nametype) ) return i / PROPS;
    }
    return 0;
}

void editor_dump_class(FILE *out, const char *mask) {
    if( out ) {
        int editor_num_rows = array_count(matrix) / PROPS;

        fprintf(out, "[editor.classes]\n");
        for( int col = 0; col < PROPS; ++col ) {
            fprintf(out, "%s%s", col ? ",":"", (const char*)matrix[0*PROPS+col]);
        }
        fprintf(out, "\n");
        for( int row = 1, rows = editor_num_rows; row < rows; ++row ) {
            var_t (*fn)() = matrix[row*PROPS+NAME];
            const char *nametype = fn().str;
            if( !strmatchi(nametype, mask)) continue;

            for( int col = 0; col < PROPS; ++col ) {
                const void *addr = matrix[row*PROPS+col];
                const char *text = addr ? symb[row*PROPS+col] : NULL;
                fprintf(out, "%s%s", col ? ",":"", text ? text : "");
            }
            fprintf(out, "\n");
        }
    }
}

// ----------------------------------------------------------------------------

STATIC_ASSERT(sizeof(class_t) == (sizeof(void*)) * PROPS);

int (editor_add_method)(const char *classname, int m, void *pfn, const char *pfnstr) {
    static const char *last = ""; int t = editor_find_row(classname);
    if(strcmp(last,classname)) last = classname, t = editor_push_row(NULL);

    matrix[ t*PROPS + m ] = pfn;
    symb[ t*PROPS + m ] = pfnstr;
    return t;
}
class_t editor_class(const char *classname) {
    unsigned typerow = editor_find_row(classname);
    assert((typerow * PROPS + 0) < array_count(matrix));
    class_t c = {0};
    memcpy(&c, matrix + typerow * PROPS, sizeof(class_t));
    return c;
}

#endif
