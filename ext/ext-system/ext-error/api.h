#if !CODE
extern
#endif

threadlocal_ const char * error;

/*
// demo
#include <math.h>
float divf(float a, float b) {
    error = 0;

    if( !b ) return error = "divide by 0", NAN;
    return a / b;
}
#include <stdio.h>
int main() {
    float d = divf(3, 0);
    if(error) puts(error);
    printf("%f\n", d);
}

// bonus points
errors that start with a number, like "404: file not found",
can be also checked like `int errornumber = atoi(error);`.
*/
