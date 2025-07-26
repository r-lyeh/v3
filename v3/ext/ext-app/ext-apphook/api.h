void api_hook(void (*begin)(void*), void(*end)(void*), void *ctx);
void  api_begin();
void  api_end();
void api_unhook(void (*begin)(void*), void(*end)(void*), void *ctx);

#if CODE

int apis;

struct {
    void *ctx;
    void (*begin)(void*);
    void (*end)(void*);
} api[64] = {0};

void api_unhook(void (*begin)(void*), void(*end)(void*), void *ctx) {
    for( int i = 0; i < apis; ++i ) {
        if( api[i].ctx == ctx )
        if( api[i].begin == begin )
        if( api[i].end == end ) {
            memmove(&api[i], &api[i+1], (64 - (i+1)) * sizeof(api[0]));
            apis--;
            return;
        }
    }
}
void api_hook(void (*begin)(void*), void(*end)(void*), void *ctx) {
    api_unhook(begin, end, ctx);
    if( apis < 64 ) {
        api[apis  ].ctx = ctx;
        api[apis  ].begin = begin;
        api[apis++].end = end;
    }
}
void api_begin() {
    for( int i = 0; i < apis; ++i ) {
        if( api[i].begin )
            api[i].begin(api[i].ctx);
    }
}
void api_end() {
    for( int i = 0; i < apis; ++i ) {
        if( api[i].end )
            api[i].end(api[i].ctx);
    }
}

#endif
