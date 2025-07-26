unsigned strcnt(const char *text, char ch); // counts occurences of character in text

#if CODE
#pragma once

unsigned strcnt(const char *text, char ch) {
    unsigned count = 0;
    while(*text++) count += text[-1] == ch;
    return count;
}

#endif
