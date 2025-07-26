// very small object class.
// you can construct the object in-place on top of the returned memory.
// you can ~destruct() the object and also check against class type.
// @todo: there are 16 free bits (16 MSB) in each function pointer.
// @todo: there are 19 free bits (16 MSB + 3 LSB) in each stored pointer to datas.
//        see: unused bits in 48-bit virtual addressing and 8-byte alignment.
//        eg: obj *o = make(128, NULL); ((uint64_t*)o)[-1] |= 4<<48ull|2; // alter flags

void *make_(unsigned bytes, void (*destructor)(void*)) {
    void **p = resize(0, bytes + sizeof(destructor) ), *o = p+1;
    printf("allocating  %p > constructing %p...\n", p, o);
    return p[0] = destructor, o;
}
void drop_(void *o) {
    void **p = o; --p;
    printf("destructing %p > deallocating %p... (flags: %016llx)\n", o, p, ((uintptr_t)p[0]) & ~0x0000FFFFFFFFFFF8ULL);
    void (*dtor)(void*) = (void*)(((uintptr_t)p[0]) & 0x0000FFFFFFFFFFFFULL);
    if( dtor ) dtor(o);
    resize(p, 0);
}
bool same_(void *o, void (*destructor)(void*)) { // returns true if `p` belongs to `destructor`; if so, `p` and `~()` are both of same type/class
    void **p = o; --p;
    void (*dtor)(void*) = (void*)(((uintptr_t)p[0]) & 0x0000FFFFFFFFFFFFULL);
    return dtor == destructor;
}
