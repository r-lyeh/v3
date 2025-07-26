// -----------------------------------------------------------------------------
// ## string interning (quarks)
// - rlyeh, public domain.

#ifndef QUARK_H
#define QUARK_H

// quick api:

V3_API unsigned    quark_intern( const char *string );
V3_API const char *quark_string( unsigned key );

// general api:

typedef struct quark_db_t {
    array_(char) blob;
    array_(struct quark2i) entries;
} quark_db_t;

V3_API unsigned    quarks_intern( quark_db_t*, const char *string );
V3_API const char *quarks_string( quark_db_t*, unsigned key );

#elif CODE
#pragma once

// -----------------------------------------------------------------------------
// quarks

struct quark2i {
    int offset, slen; // offset in blob + string length.
};

unsigned quarks_intern( quark_db_t *q, const char *string ) {
    if( string && string[0] ) {
        int slen = strlen(string);
        int qlen = array_count(q->blob);
        char *found;
        if( !qlen ) {
            array_resize(q->blob, slen + 1 );
            memcpy(found = q->blob, string, slen + 1);
        } else {
            found = strstr(q->blob, string);
            if( !found ) {
                array_resize(q->blob, qlen - 1 + slen + 1);
                memcpy(found = q->blob + qlen - 1, string, slen + 1 );
            }
        }
        // already interned? return that instead
        int offset = found - q->blob;
        for( int i = 0; i < array_count(q->entries); ++i ) {
            if( offset == q->entries[i].offset )
                if( slen == q->entries[i].slen )
                    return i+1;
        }
        // else cache and return it
        array_push(q->entries, (ifdef(V3_C,(struct quark2i),struct quark2i){offset, slen}));
        return array_count(q->entries);
    }
    return 0;
}
const char *quarks_string( quark_db_t *q, unsigned key ) {
    if( key && key <= array_count(q->entries) ) {
        struct quark2i found = q->entries[key-1];
        return va("%.*s", found.slen, q->blob + found.offset);
    }
    return "";
}

static threadlocal_ quark_db_t qdb;
unsigned quark_intern( const char *string ) {
    return quarks_intern( &qdb, string );
}
const char *quark_string( unsigned key ) {
    return quarks_string( &qdb, key );
}

#endif
