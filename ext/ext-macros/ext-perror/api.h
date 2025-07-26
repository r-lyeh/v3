#ifndef PERROR // extended perror() that prints file:line too
#define PERROR(x) perror(JOIN(__FILE__":",STRINGIZE(__LINE__)) " " x)
#endif
