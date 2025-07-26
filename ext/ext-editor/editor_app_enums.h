#ifndef EDITOR_EVENT_H
#define EDITOR_EVENT_H

#define FOREACH_EDITOR_EVENT(ON) \
    ON(BOOT)ON(POOL)ON(QUIT) /* first init, reuse init, quit*/\
    ON(TICK)ON(DRAW)  /* tick and draw */\
    ON(REST)ON(WAKE)  /* push asset state to disk > free memory; realloc memory > resume state from disk and */\
    ON(UNDO)ON(REDO)  /* */\
    ON(SAVE)ON(LOAD)  /* how editor saves/loads its state and or external asset. different from above*/\
    ON(DRAG)ON(DROP)  /* transition of urls from/to other editors (~ipc) */\
    ON(PUTS)ON(GETS)  /* how warnings are logged and how console commands are received */\
    ON(GROW)         /* how editor de/allocates their memory (~realloc) */\
    ON(BIND)         /* how editor is controlled: input events, bindings and shorcuts */\
    ON(COST)         /* how editor profiles assets and related costs*/\
    ON(DEPS)ON(SYMS)  /* all editor dependencies, and the scripting symbols exported */
//  ON(DLL)ON(VERSION)\ // reload and/or specify dll, api version provided (v3)
//  ON(MENU)ON(ICON)ON(MICE) // define menus, icons and mouse cursors
//  ON(IMPORT)ON(EXPORT) // foreign asset ex/importing

typedef struct editor_events {
    #define DEFINE_FUNCTOR(X) void (*ON_##X)();
    FOREACH_EDITOR_EVENT(DEFINE_FUNCTOR)
    #undef DEFINE_FUNCTOR
} editor_events, editor_events_t;

#endif
