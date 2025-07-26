#define V3_IMPLEMENTATION
#include "../v3/v3.h"

int main() {
    puts("v3, hello from C");

    for(script_t *L = script_create(); L; script_destroy(L), L = 0)
        script_runfile(L, "hello.lua");

    for( app_create("v3", "0.75 GL2"); app_swap(); ) {
        glViewport(0, 0, app_width(), app_height());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_BLEND);

        glMatrixMode(GL_PROJECTION);
        glRotatef(0.1f, 0, 0, 1);

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.6f, -0.75f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.6f, -0.75f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.0f, 0.75f);
        glEnd();

        if( ui_button("click") ) {
            puts("clicked");
        }
    }

    return 0;
}
