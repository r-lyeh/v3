#define V3_IMPLEMENTATION
#include "v3.h"
#include <stdlib.h>
#include <stdio.h>

#include "glfw3undecorated.h"

int main(void) {
    if (!glfwInit()) {
        exit(1);
    }

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Resizable Undecorated Window Demo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    event(on_graphics_context_created);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if(!glfwResizeUndecoratedWindow(window,~0u))
            glfwMoveUndecoratedWindow(window);

        // Simple rendering (clear to color)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(0);
}
