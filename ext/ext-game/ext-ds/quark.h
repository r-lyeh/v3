// -----------------------------------------------------------------------------
// ## string interning (quarks)
// - rlyeh, public domain.

#ifndef QUARK_H
#define QUARK_H

typedef struct quarkdb_t {
    array_(char) blob;
    array_(struct quark) entries;
} quarkdb_t;

V3_API unsigned    qintern( const char *string, quarkdb_t* ); // quarktable may be NULL
V3_API const char *qstring( unsigned key, quarkdb_t* );       // quarktable may be NULL

#define quark(str) qintern(str, NULL)

#elif CODE
#pragma once

// -----------------------------------------------------------------------------
// quarks

struct quark {
    int offset, slen; // offset in blob + string length.
};

static threadlocal_ quarkdb_t qdb;

unsigned qintern( const char *string, quarkdb_t *q ) {
    if( !q ) q = &qdb;
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
        array_push(q->entries, (ifdef(V3_C,(struct quark),struct quark){offset, slen}));
        return array_count(q->entries);
    }
    return 0;
}
const char *qstring( unsigned key, quarkdb_t *q ) {
    if( !q ) q = &qdb;
    if( key && key <= array_count(q->entries) ) {
        struct quark found = q->entries[key-1];
        return va("%.*s", found.slen, q->blob + found.offset);
    }
    return "";
}

#endif
