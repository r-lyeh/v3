#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    for( app_create(.title = "dd(grid)", .scale = 0.75); app_swap(); ) {

        static float fov = 45; ui_float("fov", &fov, 1, 100);
        float aspect = app_width() / (float)app_height();
        mat44 proj = perspective44(fov, aspect, 0.25, 1024);
        mat44 view = lookat44(vec3(0,5,-sin(time_ss())*5), vec3(0,0,0), vec3(0,1,0));

        dd_gridinf(ptr44(proj), ptr44(view));
    }

    return 0;
}
