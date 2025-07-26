extern double (*eval)(const char *expr);

#if CODE
#pragma once
#include v3_eval_backend

double (*eval)(const char *expr) = eval_;
#endif
