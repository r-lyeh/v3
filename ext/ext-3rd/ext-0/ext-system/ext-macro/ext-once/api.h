#ifndef ONCE // execute next scope only once
#define ONCE static int MACRO(once) = 1; if(MACRO(once)) if(MACRO(once)=0,1) // for(;MACRO(once);MACRO(once)=0)
#endif
