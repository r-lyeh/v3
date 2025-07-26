const char *type_(void *obj) {
    uintptr_t bits = *(uintptr_t*)obj;
    uintptr_t flag = bits & ~0x0000FFFFFFFFFFF8ULL;
    uintptr_t addr = bits &  0x0000FFFFFFFFFFF8ULL;
    void **vt = (void**)addr;
    void (*fn)(const char **) = vt['T'];
    const char *t; fn(&t);
    return t;
}
const char *echo_(void *obj) {
    uintptr_t bits = *(uintptr_t*)obj;
    uintptr_t flag = bits & ~0x0000FFFFFFFFFFF8ULL;
    uintptr_t addr = bits &  0x0000FFFFFFFFFFF8ULL;
    void **vt = (void**)addr;
    void (*fn)(void *, const char **) = vt['E'];
    const char *e; fn(obj, &e);
    return e;
}
bool same_(void *obj1, void *obj2) {
    return 0 == strcmp(type_(obj1), type_(obj2));
}
void drop_(void *obj) {
    uintptr_t bits = *(uintptr_t*)obj;
    uintptr_t flag = bits & ~0x0000FFFFFFFFFFF8ULL;
    uintptr_t addr = bits &  0x0000FFFFFFFFFFF8ULL;
    void **vt = (void**)addr;

    void (*fn)(void*) = vt['D'];
    if(fn) fn(obj); // calls destructor
    if(flag & 1) free(obj); // free heap memory if needed
}
