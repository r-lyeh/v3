// xml api

int             xml_push(const char *xml_content);
const char *        xml_string(char *key);
unsigned            xml_count(char *key);
//array(char)       xml_base64(char *key);
#define             xml_string(...)      xml_string(va(__VA_ARGS__))  // syntax sugar: string
#define             xml_int(...)    atoi(xml_string(__VA_ARGS__))     // syntax sugar: int
#define             xml_float(...)  atof(xml_string(__VA_ARGS__))     // syntax sugar: float
//#define           xml_base64(...)      xml_base64(va(__VA_ARGS__))  // syntax sugar: base64 blob
#define             xml_count(...)       xml_count(va(__VA_ARGS__))   // syntax sugar: count nodes
void            xml_pop();

// xml impl

static __thread array(char *) xml_sources;
static __thread array(struct xml *) xml_docs;

int xml_push(const char *xml_source) {
    if( xml_source ) {
        char *src = STRDUP(xml_source), *error = 0;
        for( struct xml *doc = xml_parse(src, 0, &error); doc && !error; ) {
            array_push(xml_docs, doc);
            array_push(xml_sources, src);
            return 1;
        }
        if( error ) PRINTF("%s\n", error);
        FREE(src);
    }
    return 0;
}

void xml_pop() {
    if( array_count(xml_docs) ) {
        xml_free( *array_back(xml_docs) );
        array_pop(xml_docs);
        FREE( *array_back(xml_sources) );
        array_pop(xml_sources);
    }
}

static void *xml_path(struct xml *node, char *path, int down) {
    if( !path || !path[0] ) return node;
    if( node ) {

        char type = path[0];
        if( type == '/' ) {
            int sep = strcspn(++path, "/[@$");
            if( !sep ) type = path[0];
            else
            if( 1 ) { // path[ sep ] ) {
                char tag[32]; snprintf(tag, 32, "%.*s", sep, path);
                // Find the first sibling with the given tag name (may be the same node)
                struct xml *next = down ? xml_find_down(node, tag) : xml_find(node, tag);
                return xml_path(next, &path[ sep ], 1);
            }
        }
        if( type == '$' ) {
            return (void*)( node->down ? xml_text( node->down ) : xml_tag( node ) );
        }
        if( type == '@' ) {
            return (void*)xml_att(node, ++path);
        }
        if( type == '[' ) {
            for( int i = 0, end = atoi(++path); i < end; ++i ) { node = xml_find_next(node, xml_tag(node)); if(!node) return NULL; }
            while( isdigit(path[0]) ) ++path;
            return xml_path(node, ++path, 1);
        }
    }
    return NULL;
}

const char *(xml_string)(char *key) {
    struct xml *node = xml_path(*array_back(xml_docs), key, 0);
    if( node && strchr(key, '@') ) return (const char *)node;
    if( node && strchr(key, '$') ) return (const char *)node;
    return "";
}
unsigned (xml_count)(char *key) {
    struct xml *node = xml_path(*array_back(xml_docs), key, 0);
    if( !node ) return 0;
    const char *tag = xml_tag(node);
    unsigned count = 1;
    while( (node = xml_find_next(node, tag)) != 0) ++count;
    return count;
}
array(char) (xml_base64)(char *key) { // base64 blob
    struct xml *node = xml_path(*array_back(xml_docs), key, 0);
    if( !node ) return 0;
    if( !strchr(key, '$') ) return 0;
    const char *data = (const char*)node;
    array(char) out = base64_decode(data, strlen(data)); // either array of chars (ok) or null (error)
    return out;
}

