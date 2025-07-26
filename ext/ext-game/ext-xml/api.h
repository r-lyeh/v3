// -----------------------------------------------------------------------------
// xml api

int             xml_push(const char *xml_contents);
int             xml_pushf(const char *xml_pathfile);
int                 xml_int(const char *key);
double              xml_float(const char *key);
const char *        xml_string(const char *key);
unsigned            xml_count(const char *key);
//array_(char)      xml_base64(char *key);
void            xml_pop();

#if CODE
#pragma once
#include v3_xml_backend
#endif
