// tells the pipeline how to cook an asset. for each `extension masks` that apply, run the `script` that transforms input. with additional `ziplevel` compression
extern int (*recipe)(const char *extmasks, int ziplevel, const char *script);

// redirects any given file to its cooked counterpart.
// if file is uncooked and a recipe in cookbook exists, then cook file in between.
extern const char* (*cook)(const char *filename);

#if CODE
#pragma once
#include v3_cook_backend

int (*recipe)(const char *, int, const char *) = recipe_;
const char* (*cook)(const char *) = cook_;
#endif
