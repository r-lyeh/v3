unsigned texture(image_t img);
void texture_destroy(unsigned *texture_id);

void sprite(unsigned texture_id, float x, float y, float w, float h);

#if CODE
#pragma once

unsigned texture(image_t img) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, img.channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, img.data);
    return texture;
}
void texture_destroy(unsigned *texture_id) {
    glDeleteTextures(1, texture_id);
    *texture_id = 0;
}

unsigned sprite_program() {
    // Vertex shader
    const char* sprite_vs = "#version 330 core\n"
        "layout(location=0) in vec2 pos;\n"
        "layout(location=1) in vec2 texCoord;\n"
        "out vec2 vTexCoord;\n"
        "void main() { gl_Position = vec4(pos, 0.0, 1.0); vTexCoord = texCoord; }\n";

    // Fragment shader
    const char* sprite_fs = "#version 330 core\n"
        "in vec2 vTexCoord;\n"
        "out vec4 fragColor;\n"
        "uniform sampler2D tex;\n"
        "void main() { fragColor = texture(tex, vTexCoord); }\n";

    // Compile shaders
    unsigned prg = program(2, shader('v', 1, &sprite_vs), shader('f', 1, &sprite_fs));
    return prg;
}

void sprite(unsigned texture_id, float x, float y, float w, float h) {
#if 0
    x -= w/2; // mid-anchor h
    y -= h/2; // mid-anchor v
#endif

    float aw = app_width(), ah = app_height();
    x /=  aw; // [0..1]
    x *= 2;   // [0..2]
    x--;      // [-1..1]

    y /= ah;  // [0..1]
    y *= 2;   // [0..2]
    y--;      // [-1..1]
    y = -y;   // flip

    w /= aw;
    h /= ah;

    static unsigned prg; ONCE prg = sprite_program();
    glUseProgram(prg);

    // Create and bind VAO, VBO, EBO
    static unsigned VAO, VBO, EBO;
    ONCE {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    }

    glBindVertexArray(VAO);

    {
    // Vertex data for a textured quad (positions and texture coordinates)
    float vertices[] = {
        x-w/2,y-h/2, 0.0f,0.0f, // vec2 pos, uv; // Bottom-left
        x+w/2,y-h/2, 1.0f,0.0f, // vec2 pos, uv; // Bottom-right
        x+w/2,y+h/2, 1.0f,1.0f, // vec2 pos, uv; // Top-right
        x-w/2,y+h/2, 0.0f,1.0f  // vec2 pos, uv; // Top-left
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    // Vertex attributes (position and texture coordinates)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 2 triangles (quad)
    ONCE {
    const unsigned indices[] = { 0, 1, 2, 2, 3, 0 };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glUseProgram(0);

#if 0 // @leak
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(prg);
#endif
}

#endif
