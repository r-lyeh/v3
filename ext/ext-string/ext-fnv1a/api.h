extern uint64_t (*fnv1a)(const void *ptr, unsigned len);

#if CODE
#pragma once

uint64_t fnv1a_(const void* ptr_, unsigned len) {
    const uint8_t *ptr = (const uint8_t *)ptr_;
    uint64_t hash = 14695981039346656037ULL; // hash(0),mul(131) faster than fnv1a, a few more collisions though
    while( len-- ) hash = ( *ptr++ ^ hash ) * 0x100000001b3ULL;
    return hash;
}

uint64_t (*fnv1a)(const void *ptr, unsigned len) = fnv1a_;
#endif
