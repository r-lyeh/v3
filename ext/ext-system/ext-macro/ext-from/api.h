extern threadlocal_ int          FROM_LINE;
extern threadlocal_ const char * FROM_FILE;
extern threadlocal_ const char * FROM_FUNC;

#ifndef from_
#define from_(...) ( FROM_FILE = __FILE__, FROM_LINE = __LINE__, FROM_FUNC = __func__, __VA_ARGS__)
#endif

#if CODE
#pragma once

threadlocal_ int          FROM_LINE = 0;
threadlocal_ const char * FROM_FILE = "";
threadlocal_ const char * FROM_FUNC = "";
#endif

#if 0
// how to use (header)
int mylog(const char *s);
#define mylog(...) from_(mylog(__VA_ARGS__))

// how to use (implementation)
int (mylog)(const char *s) {
    return printf("%s [%s:%s:%d]\n", s, FROM_FUNC, FROM_FILE, FROM_LINE);
}
int main() {
    mylog("hello world");
}
#endif
