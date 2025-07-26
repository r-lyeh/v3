#define expr eval_expr_ // lua already defines expr
#define eval eval_
#include "3rd/eval.c"
#undef  eval
#undef  expr
