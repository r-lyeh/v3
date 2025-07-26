// very small object class: 8-byte overhead, 2-indirection lookups per call.
// throws compile time error if vt table not the first member in a struct, or when missing.
// @todo: object composition and ECS: there are 19 free bits (16 MSB + 3 LSB) in each vt pointer.
//   see: unused bits in 48-bit virtual addressing and 8-byte alignment.

#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h> // realloc
#include <string.h> // memcpy
#include <stddef.h> // offsetof()

#define make(type,...)  ( ifdef(V3_C,(type),type){ ( (void)sizeof(char [ offsetof(type,vt) == 0 ? 1 : -1 ]), (void*)((uintptr_t)type##_vt | 0) ), ##__VA_ARGS__ })
#define heap(type,...)  ( (type*)memcpy(realloc(0, sizeof(type)), &((ifdef(V3_C,(type),type) {     ( (void)sizeof(char [ offsetof(type,vt) == 0 ? 1 : -1 ]), (void*)((uintptr_t)type##_vt | 1) ), ##__VA_ARGS__ })), sizeof(type)))

// our object generics. these are not part of the object system, but of our framework:

extern const char *(*echo)(void *obj);
extern const char *(*type)(void *obj);
extern bool        (*same)(void *obj, void *obj2);
extern void        (*drop)(void *obj); // calls destructor, and frees heap memory if needed

// syntax sugar to define quickly an object per our spec:

#define TYPEDEF_STRUCT(type, ...) typedef struct type##_t { void *vt; __VA_ARGS__ } type##_t; extern void (*type##_t_vt[])(); type##_t type(type##_t);

#elif CODE
#pragma once
#include v3_object_backend

const char *(*echo)(void*) = echo_;
const char *(*type)(void*) = type_;
bool        (*same)(void*, void*) = same_;
void        (*drop)(void*) = drop_;
#endif
