const char* strvalid(const char *text); // prevents passing null string when inlined

#if CODE
#pragma once

const char* strvalid(const char *text) {
    return text ? text : "";
}

#endif
