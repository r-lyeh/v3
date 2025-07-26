#ifndef SPLIT_H
#define SPLIT_H

extern char** (*strsplit)(const char *text, const char *separators, int *count); // do not free()

// syntax sugar. can break. usage: for each_string(s, "hello world", " ") puts(s);
#define each_string(var,str,seps) (char **lst = strsplit(strvalid(str), seps, 0), *var; !!(var = lst[0]); ++lst)

#elif CODE
#pragma once

#ifdef _WIN32 // better than strtok(). preserves empty strings within delimiters
char *strsep(char **sp, const char *sep) {
    if( sp && *sp && **sp ) {
        char *p = *sp + strcspn(*sp, sep), *s = *sp;
        if( *p ) *p++ = '\0';
        *sp = p;
        return s;
    }
    return NULL;
}
#endif

char** strsplit_(const char *text, const char *separators, int *count) { // do not free()
    int tlen = strlen(text);

    static threadlocal_ char **lists[16] = {0}, *bufs[16]= {0};
    static threadlocal_ int slot = 0; slot = (slot + 1) % COUNTOF(lists);
    lists[slot] = realloc(lists[slot], sizeof(char*)*(tlen+1));
    bufs[slot] = realloc(bufs[slot], tlen+1);

    int num = 0;
    char *buf = bufs[slot]; memcpy(buf, text, tlen+1);
    char **list = lists[slot];
    for( char *sep = buf, *token = strsep(&sep, separators); token; token = strsep(&sep, separators) ) {
        if( *token == '\0' ) continue; // skip blank tokens, found between two consecutive separators. note: it is desirable to have them sometimes (eg, in syntax parsers)
        *list++ = token;
        num++;
    }
    *list++ = 0;

    if( count ) *count = num;
    return lists[slot];
}

char** (*strsplit)(const char *, const char *, int *) = strsplit_;

#endif
