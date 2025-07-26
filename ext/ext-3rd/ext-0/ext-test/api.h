#ifndef TEST_H
#define TEST_H

extern int (*test)(int cond, const char *file, int line);
extern int (*failures)(void);

#ifndef NDEBUG
#define test(cond) test(!!(cond), __FILE__, __LINE__)
#define AUTOTEST AUTORUN
#else
#define test(cond) (!!(cond))
#define AUTOTEST void MACRO(test_unused)(void)
#endif

#elif CODE
#pragma once
#include v3_test_backend

int (*test)(int cond, const char *file, int line) = test_;
int (*failures)(void) = failures_;

#endif
