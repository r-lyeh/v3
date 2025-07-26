void dd_gridinf(float proj[16], float view[16]) {

    static GLuint vao;
    static GLuint vbo;
    static GLuint ebo;
    static GLuint prg;
    static GLint  prg_view;
    static GLint  prg_proj;

    ONCE {
        static const GLfloat vertices[] = {
            1, 1, 0, // top right
            1,-1, 0, // bottom right
           -1,-1, 0, // bottom left
           -1, 1, 0, // top left
        };

        static const GLuint indices[] = {
            0, 1, 3, // 1st triangle
            1, 2, 3, // 2nd triangle
        };

        const char *vs[] = { asset("dd_grid.vert").str };
        const char *fs[] = { asset("dd_grid.frag").str };

        prg = program(2, shader('v', 1, vs), shader('f', 1, fs));
        prg_proj = glGetUniformLocation(prg, "proj");
        prg_view = glGetUniformLocation(prg, "view");

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    glUseProgram(prg);
    glUniformMatrix4fv(prg_proj, 1, GL_FALSE, (const GLfloat *)proj);
    glUniformMatrix4fv(prg_view, 1, GL_FALSE, (const GLfloat *)view);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
}
