#ifndef JOIN // joins two tokens at compile time
#define JOIN(x,y) J0IN(x,y)
#define J0IN(x,y) x##y
#endif
