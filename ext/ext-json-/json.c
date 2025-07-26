typedef union json_t { char* s; double f; int64_t i; uintptr_t p; array(union json_t) arr; } json_t;

// json api

bool            json_push(const char *json_content);
const char*         json_key(const char *keypath);
json_t*             json_find(const char *type_keypath);
json_t              json_get(const char *type_keypath);
int                 json_count(const char *keypath);
#define             json_int(...)    (json_get(va("i" __VA_ARGS__)).i)
#define             json_float(...)  (json_get(va("f" __VA_ARGS__)).f)
#define             json_string(...) (json_get(va("s" __VA_ARGS__)).s)
#define             json_key(...)     json_key(va(__VA_ARGS__))
#define             json_count(...)   json_count(va(__VA_ARGS__))
bool            json_pop();

// ----------------------------------------------------------------------------

static array(json5) roots;
static array(char*) sources;

bool json_push(const char *source) {
    char *source_rw = STRDUP(source);
    json5 root = {0};
    char *error = json5_parse(&root, source_rw, 0);
    if( error ) {
        FREE(source_rw);
        return false;
    } else {
        array_push(sources, source_rw);
        array_push(roots, root);
        return true;
    }
}

bool json_pop() {
    if( array_count(roots) > 0 ) {
        FREE(*array_back(sources));
        array_pop(sources);

        json5_free(array_back(roots));
        array_pop(roots);
        return true;
    }
    return false;
}

json5* json_node(const char *keypath) {
    json5 *j = array_back(roots), *r = j;
    for each_substring( keypath, "/[.]", key ) {
        r = 0;
        /**/ if( j->type == JSON5_ARRAY ) r = j = &j->array[atoi(key)];
        else if( j->type == JSON5_OBJECT && isdigit(key[0]) )
        for( int i = 0, seq = atoi(key); !r && i < j->count; ++i ) {
            if( i == seq ) {
                r = j = &j->nodes[i];
                break;
            }
        }
        else if( j->type == JSON5_OBJECT )
        for( int i = 0; !r && i < j->count; ++i ) {
            if( j->nodes[i].name && !strcmp(j->nodes[i].name, key) ) {
                r = j = &j->nodes[i];
                break;
            }
        }
        if( !j ) break;
    }
    return r;
}

int (json_count)(const char *keypath) {
    json5* j = json_node(keypath);
    return j ? j->count : 0;
}

json_t *json_find(const char *type_keypath) {
    char type = type_keypath[0];
    const char *key = type_keypath+1;
    json5 *j = json_node(key);
    if( !j ) return NULL;

    static __thread int slot = 0;
    static __thread json_t buf[128] = {0};
    slot = (slot+1) % 128;

    json_t *v = &buf[slot];
    v->i = j ? j->integer : 0;
    if(type == 's' && (!v->p || j->type == JSON5_NULL)) v->s = ""; // if_null_string
    if(type == 'f' && j && j->type == JSON5_INTEGER) v->f = j->integer;
    return v;
}

json_t json_get(const char *type_keypath) {
    char type = type_keypath[0];
    const char *key = type_keypath+1;
    json5 *j = json_node(key);

    json_t v = {0};
    v.i = j ? j->integer : 0;
    if(type == 's' && (!v.p || j->type == JSON5_NULL)) v.s = ""; // if_null_string
    if(type == 'f' && j && j->type == JSON5_INTEGER) v.f = j->integer;
    return v;
}

const char *(json_key)(const char *keypath) {
    json5 *j = json_node(keypath);
    if( !j ) return "";
    return j->name;
}
