// microui gl3 backend. @todo: emscripten shaders.
// - rlyeh, public domain

#include <assert.h>
#include "microui_renderer.h"
#include "microui_atlas.inl"

static const char* vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec2 inPosition;\n"
    "layout(location = 1) in vec2 inTexCoord;\n"
    "layout(location = 2) in vec4 inColor;\n"
    "out vec2 TexCoord;\n"
    "out vec4 Color;\n"
    "uniform mat4 projection;\n"
    "void main() {\n"
    "    gl_Position = projection * vec4(inPosition, 0.0, 1.0);\n"
    "    TexCoord = inTexCoord;\n"
    "    Color = inColor;\n"
    "}\n";

static const char* fragmentShaderSource =
    "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "in vec4 Color;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D texture0;\n"
    "void main() {\n"
    "    float r = texture(texture0, TexCoord).r;\n"
    "    FragColor = vec4(1.0,1.0,1.0,r) * Color;\n"
    "}\n";

enum { MU_MAX_QUADS = 16384 };

static GLfloat   tex_buf[MU_MAX_QUADS *  8]; // x4 vec2f for a quad
static GLfloat  vert_buf[MU_MAX_QUADS *  8]; // x4 vec2f for a quad
static GLubyte color_buf[MU_MAX_QUADS * 16]; // x4 vec4u for a quad
static GLuint  index_buf[MU_MAX_QUADS *  6]; // x1 quad = (1tri+1tri) = (3vtx+3vtx) = 6 vtx

static int buf_idx;

void r_reset_gl(void) {
    /* reset gl state */
    glViewport(0, 0, app_width(), app_height()); //< required
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
}

static void flush(void) {
    if (buf_idx == 0) { return; }

    r_reset_gl();

    {
        static GLuint prg;
        static GLuint loc;
        static GLuint vao, vbo[3], ebo;
        static GLuint tex;

        ONCE {
            // create shaders and uniform location
            prg = program(2, shader("vs",1,&vertexShaderSource),shader("fs", 1, &fragmentShaderSource));
            loc = glGetUniformLocation(prg, "projection");
            assert(glGetError() == 0);

            // create texture
            glGenTextures(1, &tex);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ATLAS_WIDTH, ATLAS_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            assert(glGetError() == 0);

            // create VAO, VBOs and EBO. as dynamic streams
            vao = vao_begin();
                vbo[0] = vbo_buffer(0, "!vertex2f", vert_buf, sizeof(vert_buf));
                vbo[1] = vbo_buffer(1, "!vertex2f", tex_buf, sizeof(tex_buf));
                vbo[2] = vbo_buffer(2, "!color4b", color_buf, sizeof(color_buf));
                #ifndef __EMSCRIPTEN__
                ebo = ebo_buffer("!element", index_buf, sizeof(index_buf));
                #endif
            vao_end();
        }

        // bind shading program
        glUseProgram(prg);

        // push projection 2d matrix
        float proj[16]; // ortho44(proj, 0,app_width(), app_height(),0, -1,1);
        {
            float *m = proj;
            float l = 0, r = app_width(), b = app_height(), t = 0, n = -1, f = +1;
            m[ 0] = 2/(r-l);      m[ 1] = 0;            m[ 2] = 0;            m[ 3] = 0;
            m[ 4] = 0;            m[ 5] = 2/(t-b);      m[ 6] = 0;            m[ 7] = 0;
            m[ 8] = 0;            m[ 9] = 0;            m[10] = -2/(f-n);     m[11] = 0;
            m[12] = -(r+l)/(r-l); m[13] = -(t+b)/(t-b); m[14] = -(f+n)/(f-n); m[15] = 1;
        }
        glUniformMatrix4fv(loc, 1, GL_FALSE, proj);

        // bind texture + unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);

        // bind VAO
        glBindVertexArray(vao);

            // update vbos with new vertex data
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, buf_idx * 8 * sizeof(GLfloat), vert_buf);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, buf_idx * 8 * sizeof(GLfloat), tex_buf);
            glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, buf_idx * 16 * sizeof(GLubyte), color_buf);

            // update ebo with new index data
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, buf_idx * 6 * sizeof(GLuint), index_buf);

            // draw
            #ifndef __EMSCRIPTEN__
            glDrawElements(GL_TRIANGLES, buf_idx * 6, GL_UNSIGNED_INT, 0);
            #else
            glDrawArrays(GL_TRIANGLES, 0, buf_idx * 6);
            #endif

        // vao_destroy(vao, ifdef(ems,3,4), vbo[0], vbo[1], vbo[2], ebo);
    }

    buf_idx = 0;
}


static void push_quad(mu_Rect dst, mu_Rect src, mu_Color color) {
    if (buf_idx == MU_MAX_QUADS) { flush(); }

    int texvert_idx = buf_idx *  8;
    int   color_idx = buf_idx * 16;
    int element_idx = buf_idx *  4;
    int   index_idx = buf_idx *  6;
    buf_idx++;

    /* update texture buffer */
    float x = src.x / (float) ATLAS_WIDTH;
    float y = src.y / (float) ATLAS_HEIGHT;
    float w = src.w / (float) ATLAS_WIDTH;
    float h = src.h / (float) ATLAS_HEIGHT;
    tex_buf[texvert_idx + 0] = x;
    tex_buf[texvert_idx + 1] = y;
    tex_buf[texvert_idx + 2] = x + w;
    tex_buf[texvert_idx + 3] = y;
    tex_buf[texvert_idx + 4] = x;
    tex_buf[texvert_idx + 5] = y + h;
    tex_buf[texvert_idx + 6] = x + w;
    tex_buf[texvert_idx + 7] = y + h;

    /* update vertex buffer */
    vert_buf[texvert_idx + 0] = dst.x;
    vert_buf[texvert_idx + 1] = dst.y;
    vert_buf[texvert_idx + 2] = dst.x + dst.w;
    vert_buf[texvert_idx + 3] = dst.y;
    vert_buf[texvert_idx + 4] = dst.x;
    vert_buf[texvert_idx + 5] = dst.y + dst.h;
    vert_buf[texvert_idx + 6] = dst.x + dst.w;
    vert_buf[texvert_idx + 7] = dst.y + dst.h;

    /* update color buffer */
    memcpy(color_buf + color_idx +  0, &color, 4);
    memcpy(color_buf + color_idx +  4, &color, 4);
    memcpy(color_buf + color_idx +  8, &color, 4);
    memcpy(color_buf + color_idx + 12, &color, 4);

    /* update index buffer */
    index_buf[index_idx + 0] = element_idx + 0;
    index_buf[index_idx + 1] = element_idx + 1;
    index_buf[index_idx + 2] = element_idx + 2;
    index_buf[index_idx + 3] = element_idx + 2;
    index_buf[index_idx + 4] = element_idx + 3;
    index_buf[index_idx + 5] = element_idx + 1;
}


void r_draw_rect(mu_Rect rect, mu_Color color) {
    push_quad(rect, atlas[ATLAS_WHITE], color);
}


void r_draw_text(const char *text, mu_Vec2 pos, mu_Color color) {
    mu_Rect dst = { pos.x, pos.y, 0, 0 };
    for (const char *p = text; *p; p++) {
        if ((*p & 0xc0) == 0x80) { continue; }
        int chr = mu_min((unsigned char) *p, 127);
        mu_Rect src = atlas[ATLAS_FONT + chr];
        dst.w = src.w;
        dst.h = src.h;
        push_quad(dst, src, color);
        dst.x += dst.w;
    }
}


void r_draw_icon(int id, mu_Rect rect, mu_Color color) {
    mu_Rect src = atlas[id];
    int x = rect.x + (rect.w - src.w) / 2;
    int y = rect.y + (rect.h - src.h) / 2;
    push_quad(mu_rect(x, y, src.w, src.h), src, color);
}


int r_get_text_width(const char *text, int len) {
    int res = 0;
    for (const char *p = text; *p && len--; p++) {
        if ((*p & 0xc0) == 0x80) { continue; }
        int chr = mu_min((unsigned char) *p, 127);
        res += atlas[ATLAS_FONT + chr].w;
    }
    return res;
}


int r_get_text_height(void) {
    return 18;
}


void r_set_clip_rect(mu_Rect rect) {
    flush();
    glScissor(rect.x, app_height() - (rect.y + rect.h), rect.w, rect.h);
}


void r_clear(mu_Color clr) {
    flush();
    glClearColor(clr.r / 255.0f, clr.g / 255.0f, clr.b / 255.0f, clr.a / 255.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void r_present(void) {
    flush();
}
