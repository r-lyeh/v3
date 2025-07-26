// All actions (or verbs) that the editor can perform on any given type are defined in this header.
// The editor uses `var_t verb();` functors everywhere.
// These functors can point to external DLLs or symbols defined in current executable.

#ifndef FOREACH_TYPE_ACTION
#define FOREACH_TYPE_ACTION(X) \
    /* name and minimum size of an object */\
    X(NAME)X(SIZE) \
    /* how to save an object, how to load an object */\
    X(SAVE)X(LOAD)\
    /* how to edit an object */\
    X(EDIT)\
    /* how to hash object contents, how to compare two objects */\
    X(HASH)X(SORT)\
    /* placeholders in 2d and 3d views; tint color of an object */\
    X(ICON)X(MESH)X(TINT)\
    /* 2d/3d dimensions and enclosing shape form of an object */\
    X(SPAN)X(FORM)

enum {
    #define DEFINE_ENUM(x) x,
    FOREACH_TYPE_ACTION(DEFINE_ENUM)
    #undef DEFINE_ENUM
    PROPS            // total number of editing properties per object
};
#endif
