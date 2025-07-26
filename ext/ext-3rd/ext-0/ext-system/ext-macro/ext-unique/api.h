#ifndef UNIQUE // create unique identifier
#define UNIQUE(name) JOIN(JOIN(JOIN(name,JOIN(_L,__LINE__)),_),__COUNTER__)
#endif
