typedef array_(vec3) dd_vec3;
typedef map_(uint64_t,dd_vec3) dd_map;
dd_map *dd_list = 0;

int dd_map_cmp(const void *pa, const void *pb) {
    const dd_map *a = pa;
    const dd_map *b = pb;
    return a->key < b->key ? -1 : a->key > b->key ? 1 : 0;
}

void dd_flush(mat44 proj, mat44 view) {
    int e = map_count(dd_list);
    if(!e) return;

    unsigned restore_flags = dd_state_begin(proj, view);

    dd_flags dd_state = dd_flags();
    dd_state.bits = ~dd_state.bits; dd_state.color = 0;

    map_sort(dd_list, dd_map_cmp);

    for(int x = 0; x < e; ++x) {
        int count = array_count(dd_list[x].value);
        if( count ) {
            dd_flags f = { .bits = dd_list[x].key };
            dd_state_apply(&dd_state, &f);
            dd_commit(dd_list[x].value, count, &dd_state);
 
            array_clear(dd_list[x].value);
        }
    }

    dd_state_end(restore_flags);
}

void (dd_point)(vec3 pos, dd_flags g) {
    array_(vec3) *found = &map_find_or_add(dd_list, g.key)->value;
    for( int i = 0; i < g.count; ++i ) {
        array_push(*found, pos);
        pos = add3(pos, g.incpos);
    }
}

void (dd_line)(vec3 from, vec3 to, dd_flags g) {
    array_(vec3) *found = &map_find_or_add(dd_list, g.key)->value;
    for( int i = 0; i < g.count; ++i ) {

        if( g.dashed ) {
            vec3 dist = sub3(to, from); vec3 unit = norm3(dist);
            for( float len = 0, mag = len3(dist) / 2; len < mag; ++len ) {
                to = add3(from, unit);
                array_push(*found, from);
                array_push(*found, to);
                from = add3(to, unit);
            }
        } else {
            array_push(*found, from);
            array_push(*found, to);
        }

        from = add3(from, g.incpos);
        to   = add3(to,   g.incpos);
        g.n  = add3(g.n,  g.incn);
    }
}

void dd_clear(void) {
    for(int x = 0, e = map_count(dd_list); x < e; ++x) {
        array_clear(dd_list[x].value);
    }
}
void dd_present(void *arg) {
    static int use_backend;
    ONCE use_backend = strstri(app_state().extra, "GL2") ? 2 : 3;
    if( use_backend == 2 ) dd_clear();
    else dd_flush(last_proj, last_view);
}
AUTORUN {
    on_frame_end(dd_present, NULL);
}
