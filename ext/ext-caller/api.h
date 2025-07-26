extern threadlocal_ int          __CALLER_LINE__;
extern threadlocal_ const char * __CALLER_FILE__;
extern threadlocal_ const char * __CALLER_FUNC__;

#ifndef caller_
#define caller_(...) (  __CALLER_FILE__ = __FILE__, __CALLER_LINE__ = __LINE__, \
                        __CALLER_FUNC__ = __func__, __VA_ARGS__)
#endif

#if CODE
#pragma once

threadlocal_ int          __CALLER_LINE__ = 0;
threadlocal_ const char * __CALLER_FILE__ = "";
threadlocal_ const char * __CALLER_FUNC__ = "";
#endif

#if 0
// how to use (header)
int mylog(const char *s);
#define mylog(...) caller_(mylog(__VA_ARGS__))

// how to use (implementation)
int (mylog)(const char *s) {
    return printf("%s [%s:%s:%d]\n", s, __CALLER_FUNC__, __CALLER_FILE__, __CALLER_LINE__);
}
int main() {
    mylog("hello world");
}
#endif
