// @fixme: preserve .ini comments between sessions

// ----------------------------------------------------------------------------
// ini+, extended ini format
// - rlyeh, public domain
//
// # spec
//
//   ; line comment
//   [user]             ; map section name (optional)
//   name=john          ; key and value (mapped here as user.name=john)
//   +surname=doe jr.   ; sub-key and value (mapped here as user.name.surname=doe jr.)
//   age=30             ; numbers
//   color=240          ; key and value \
//   color=253          ; key and value |> array: color[0], color[1] and color[2]
//   color=255          ; key and value /
//   color=             ; remove key/value(s)
//   color=white        ; recreate key; color[1] and color[2] no longer exist
//   []                 ; unmap section
//   -note=keys may start with symbols (except plus `+` and semicolon `;`)
//   -note=linefeeds are either \r, \n or \r\n.
//   -note=utf8 everywhere.

static
char *ini_parse( const char *s ) {
    char *map = 0;
    int mapcap = 0, maplen = 0;
    enum { DEL, REM, TAG, KEY, SUB, VAL } fsm = DEL;
    const char *cut[6] = {0}, *end[6] = {0};
    if(s) while( *s ) {
        while( *s && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') ) ++s;
        /**/ if( *s == ';' ) cut[fsm = REM] = ++s;
        else if( *s == '[' ) cut[fsm = TAG] = ++s;
        else if( *s == '+' ) cut[fsm = SUB] = ++s;
        else if( *s == '=' ) cut[fsm = VAL] = ++s;
        else if( *s > ' ' && *s <= 'z' && *s != ']' ) cut[fsm = KEY] = cut[SUB] = end[SUB] = s;
        else { if( *s ) ++s; continue; }
        /**/ if( fsm == REM ) { while(*s && *s != '\r'&& *s != '\n') ++s; }
        else if( fsm == TAG ) { while(*s && *s != '\r'&& *s != '\n'&& *s != ']') ++s; end[TAG] = s; }
        else if( fsm == KEY ) { while(*s && *s >  ' ' && *s <= 'z' && *s != '=') ++s; end[KEY] = s; }
        else if( fsm == SUB ) { while(*s && *s >  ' ' && *s <= 'z' && *s != '=') ++s; end[SUB] = s; }
        else if( fsm == VAL ) { while(*s && *s >= ' ' && *s <= 127 && *s != ';') ++s; end[VAL] = s;
            while( end[VAL][-1] <= ' ' ) { --end[VAL]; }
            char buf[256] = {0}, *key = buf;
            if( end[TAG] - cut[TAG] ) key += sprintf(key, "%.*s.", (int)(end[TAG] - cut[TAG]), cut[TAG] );
            if( end[KEY] - cut[KEY] ) key += sprintf(key,  "%.*s", (int)(end[KEY] - cut[KEY]), cut[KEY] );
            if( end[SUB] - cut[SUB] ) key += sprintf(key, ".%.*s", (int)(end[SUB] - cut[SUB]), cut[SUB] );
            int reqlen = (key - buf) + 1 + (end[VAL] - cut[VAL]) + 1 + 1;
            if( (reqlen + maplen) >= mapcap ) map = realloc( map, mapcap += reqlen + 512 );
            sprintf( map + maplen, "%.*s%c%.*s%c%c", (int)(key - buf), buf, 0, (int)(end[VAL] - cut[VAL]), cut[VAL], 0, 0 );
            maplen += reqlen - 1;
        }
    }
    return map;
}

static threadlocal_ quarkdb_t ini_db;

typedef map_(int,int)* ini_t;

static ini_t ini_map(const char *contents) {
    ini_t map = 0;

    if( contents && contents[0] ) {
        char *kv = ini_parse(contents);
        if( kv ) {
            for( char *iter = kv; iter[0]; ) {
                char *key = iter; while( *iter++ );
                char *val = iter; while( *iter++ );

                int qkey = qintern(key, &ini_db);
                int qval = qintern(val, &ini_db);

                map_insert(map, qkey, qval);
            }
            free(kv);
        }
    }

    return map;
}

const char *ini_write_(const char *filename_section_key, const char *value) {
    const char *at0 = filename_section_key;
    const char *at1 = strstr(at0, ".ini/"); if(!at1) return NULL; at1 += 5;
    const char *at2 = strchr(at1, '/'); if(!at2) return NULL; at2 += 1;
    char buf0[260]; snprintf(buf0, 260, "%.*s", (int)(at1 - at0 - 1), at0);
    char buf1[128]; snprintf(buf1, 128, "%.*s", (int)(at2 - at1 - 1), at1);
    const char *filename = buf0;
    const char *section = buf1;
    const char *key = at2;

    // this is a little hacky {
    file_t f = file_read(filename);
    if( f.str && f.str[0] ) {
        char *data = f.str;
        char *begin = strrchr(data, '[');
        char *end = strrchr(data, ']');
        if( begin && end && begin < end ) {
            char *last_section = va("%.*s", (int)(end - begin - 1), begin + 1);
            if( !strcmpi_(last_section, section) ) section = 0;
        }
    }
    // }

    char *s = va("%s%s=%s\r\n", section ? va("[%s]\r\n", section) : "", key, value);
    return file_join(filename, s, strlen(s)) ? value : NULL;
}

const char* ini_read_(const char *filename_section_key) {
    const char *at0 = filename_section_key;
    const char *at1 = strstr(at0, ".ini/"); if(!at1) return NULL; at1 += 5;
    const char *at2 = strchr(at1, '/'); if(!at2) return NULL; at2 += 1;
    char buf0[260]; snprintf(buf0, 260, "%.*s", (int)(at1 - at0 - 1), at0);
    char buf1[128]; snprintf(buf1, 128, "%.*s", (int)(at2 - at1 - 1), at1);
    const char *filename = buf0;
    const char *section = buf1;
    const char *key = at2;

    ini_t map = ini_map(file_read(filename).str);

    int qkey = qintern( section && section[0] ? va("%s.%s", section, key) : key, &ini_db );
    ini_t found = map_find( map, qkey );
    int qval = found ? found->value : -1;

    map_free(map);

    return qval >= 0 ? qstring( qval, &ini_db ) : NULL;
}

static threadlocal_ array_(ini_t) ini_queue;

int ini_push_(const char *contents) {
    ini_t map = ini_map(contents);
    array_push(ini_queue, map);
    return 1;
}
int ini_pushf_(const char *filename) {
    return ini_push_(file_read(filename).str);
}
int ini_pop_(void) {
    if( array_count(ini_queue) ) {
        ini_t *map = array_pop(ini_queue);
        map_free(*map);
        return 1;
    }
    return 0;
}

int ini_count_(void) {
    ini_t *map = array_back(ini_queue);
    int count = map_count(*map);
    return count;
}

const char *ini_key_(int id) {
    const char *rc = 0;
    if( id >= 0 ) {
        ini_t *map = array_back(ini_queue);
        int count = map_count(*map);
        if( id < count ) rc = qstring( (*map)[id].key, &ini_db );
    }
    return rc;
}

const char *ini_value_(int id) {
    const char *rc = 0;
    if( id >= 0 ) {
        ini_t *map = array_back(ini_queue);
        int count = map_count(*map);
        if( id < count ) rc = qstring( (*map)[id].value, &ini_db );
    }
    return rc;
}
