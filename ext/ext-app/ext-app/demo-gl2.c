#define V3_IMPLEMENTATION
#include "v3.h"

int main() {

    for( app_create(.title = "v3", .scale = 0.75, .extra = "GL2"); app_swap(); ) {
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

//      if( ui_button("click") ) {
//          puts("clicked");
//      }
    }

    return 0;
}
