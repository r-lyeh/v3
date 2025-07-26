// All actions (or verbs) that the editor can perform on any given type are defined in this header.
// The editor uses `var_t verb();` functors everywhere.
// These functors can point to external DLLs or symbols defined in current executable.

#ifndef EDITOR_FOREACH_VERB
#define EDITOR_FOREACH_VERB(X) \
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
    #define EXPAND_ENUM(x) x,
    EDITOR_FOREACH_VERB(EXPAND_ENUM)
    #undef EXPAND_ENUM
    PROPS            // total number of editing properties per object
};
#endif
