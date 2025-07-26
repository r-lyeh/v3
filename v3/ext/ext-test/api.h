#ifndef TEST_H
#define TEST_H

extern int (*test)(int cond, const char *file, int line);

#ifdef NDEBUG
#define test(cond) (cond)
#define UNITTEST void MACRO(test_unused)(void)
#else
#define test(cond) test((cond), __FILE__, __LINE__)
#define UNITTEST AUTORUN
#endif

#elif CODE
#pragma once
#include v3_test_backend

int (*test)(int cond, const char *file, int line) = test_;

UNITTEST {
    test(1 == 1 && "ensure unit-tests do work");
}

#endif
