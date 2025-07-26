#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    const char *vert =
        "#version 330 core\n"
        "layout (location = 0) in vec3 attPos;\n"
        "out vec3 vertColor;\n"
        "uniform mat4 transform;\n"
        "void main() {\n"
        "   gl_Position = transform * vec4(attPos, 1.0);\n"
        "   vec3 array[3] = { vec3(1,0,0), vec3(0,1,0), vec3(0,0,1) };"
        "   vertColor = array[gl_VertexID];\n"
        "}";

    const char *frag =
        "#version 330 core\n"
        "in vec3 vertColor;\n"
        "out vec4 fragColor;\n"
        "void main() { fragColor = vec4(vertColor, 1.0); }";

    const float positions[] = {
         0.0f,  0.5f, 0.0f, // top
        -0.5f, -0.5f, 0.0f, // bottom Left
         0.5f, -0.5f, 0.0f, // bottom Right
    };

    app_create(.title = "v3", .scale = 0.75);

    unsigned prg = program(2, shader('v', 1, &vert), shader('f', 1, &frag));
    unsigned vao = vao_begin();
    unsigned vbo = vbo_buffer(0, "vertex3f", positions, sizeof(positions));
    vao_end();

    for( int frame = 0; app_swap(); ++frame ) {
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float angle = frame / 1000.f, ca = cosf(angle), sa = sin(angle);
        float transform[16] = { ca,-sa,0,0, sa,ca,0,0, 0,0,1,0, 0,0,0,1 };
        glUseProgram(prg);
        glUniformMatrix4fv(glGetUniformLocation(prg, "transform"), 1, GL_FALSE, transform);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //ui_label(va("app(%d,%d) mouse(%d,%d)", app_width(), app_height(), mouse('x'), mouse('y'))); // (int)fps(), (int)hz()));
    }

    program_destroy(prg);
    vao_destroy(vao, 1, vbo);
    app_destroy();
    return 0;
}
