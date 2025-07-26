typedef void* script_t;

extern script_t(*script_create)();
extern bool    (*script_preload)(script_t, const char *modname, const char *modcode);
extern bool    (*script_runfile)(script_t,const char *fname);
extern bool    (*script_runstring)(script_t,const char *str);
extern void    (*script_destroy)(script_t);

#include v3_script_backend

#if CODE
#pragma once
script_t(*script_create)() = (void*)script_create_;
bool    (*script_preload)(script_t, const char *modname, const char *modcode) = (void*)script_preload_;
bool    (*script_runfile)(script_t,const char *fname) = (void*)script_runfile_;
bool    (*script_runstring)(script_t,const char *str) = (void*)script_runstring_;
void    (*script_destroy)(script_t) = (void*)script_destroy_;
#endif
