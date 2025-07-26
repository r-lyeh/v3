#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    app_create();

    // Load texture
    image_t img = image(.url = "v3.png", .flip = 1);
    if (!img.w) { printf("Failed to load texture\n"); glfwTerminate(); return -1; }
    unsigned texture_id = texture(img);
    image_drop(&img);


    // Main loop
    while (app_swap()) {
        glClear(GL_COLOR_BUFFER_BIT);

        sprite(texture_id, mouse('x'),mouse('y'), 64,64);
    }

    // Cleanup
    texture_destroy(&texture_id);
    return 0;
}
