#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    for( app_create(.extra = "GL2"); app_swap(); ) {
        
        CPU_PROFILE("section fast") {
            printf("\0hi");
        }

        CPU_PROFILE("section slow") {
            for( int i = 0; i < 1000000; ++i );
        }

        GPU_PROFILE("GL clear") {
            glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_BLEND);
        }

        GPU_PROFILE("GL triangle") {
            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.6f, -0.75f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.6f, -0.75f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.0f, 0.75f);
            glEnd();
        }
    }

    return 0;
}
