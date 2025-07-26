// @todo: read https://mattdesl.svbtle.com/drawing-lines-is-hard

#pragma once

static int      dd_prg;
static int      dd_prg_color;
static int      dd_prg_mvp;

static int      dd_prg2;
static int      dd_prg2_color;
static int      dd_prg2_mvp;

static mat44    dd_mvp;

static const char *preamble = ifdef(V3_MACOS, "#version 150\n", "#version 130\n");

static const char *dd_vs = "//" FILELINE "\n"
    "in vec3 att_position;\n"
    "uniform mat4 u_MVP;\n"
    "uniform vec3 u_color;\n"
    "out vec3 out_color;\n"
    "out float stipple_coord;\n"
    "void main() {\n"
    "    gl_Position = u_MVP * vec4(att_position, 1.0);\n"
    "    out_color = u_color;\n"
    "    stipple_coord = length(att_position.xy); // Texture coordinate along the line\n"
    "}";

static const char *dd_fs_line_solid = "//" FILELINE "\n"
    // "precision mediump float;\n"
    "in vec3 out_color;\n"
    "out vec4 fragcolor;\n"
    "void main() {\n"
    "   fragcolor = vec4(out_color, 1.0);\n"
    "}";

static const char *dd_fs_line_stipple = "//" FILELINE "\n"
    // "precision mediump float;\n"
    "in vec3 out_color;\n"
    "in float stipple_coord;\n"
    "out vec4 fragcolor;\n"

    "uniform float stipple_scale = 10.0; // Controls pattern frequency\n"

    "// Macros for stipple patterns\n"
    "#define gap(N)  curr_pos += N;\n"
    "#define dash(N) keep_fragment = keep_fragment || (coord >= curr_pos && coord < curr_pos + N); curr_pos += N;\n"
    "#define dot(N)  keep_fragment = keep_fragment || (coord >= curr_pos && coord < curr_pos + N); curr_pos += N;\n"

    "void main() {\n"
    "    float coord = stipple_coord * stipple_scale;\n"
    "    float curr_pos = 0.0;\n"
    "    bool keep_fragment = false;\n"

    "    // note: each sum must be 1.0\n"
    "    #define pattern_dashed() dash(0.5) gap(0.5)\n"
    "    #define pattern_dot_dashed() dot(0.2) gap(0.2) dash(0.4) gap(0.2)\n"
    "    #define pattern_dot_dot() dot(0.2) gap(0.3) dot(0.2) gap(0.3)\n"

    "    // Apply pattern with fixed period of 1.0\n"
    "    coord = mod(coord, 1.0);\n"
    "    pattern_dot_dot();\n"

    "    // Draw\n"
    "    if (!keep_fragment) discard; else\n"
    "    fragcolor = vec4(out_color, 1.0);\n"
    "}\n";

// ideas from https://mmmovania.blogspot.com/2010/12/circular-point-sprites-in-opengl-33.html
static const char *dd_fs_point = "//" FILELINE "\n"
    // "precision mediump float;\n"
    "in vec3 out_color;\n"
    "out vec4 fragcolor;\n"
    "void main() {\n"
    "   vec2 p = gl_PointCoord * 2.0 - vec2(1.0); // Transform to [-1, 1]²\n"
    "   float r = sqrt(dot(p,p));\n"
    "   float theta = atan(p.y,p.x);\n"
    "   float thickness = 0.2; // thickness of some symbols below (+,x,[])\n"

    // pluses(+)
    // "   if(abs(p.x) >= thickness && abs(p.y) >= thickness) discard; else\n" // Pluses `+`

    // crosses(x)
    // "   float dist1 = abs(p.x - p.y); // Distance to diagonal line y =  x\n"
    // "   float dist2 = abs(p.x + p.y); // Distance to diagonal line y = -x\n"
    // "   if(dist1 >= thickness && dist2 >= thickness) discard; else // Keep fragments where distance to either diagonal is less than thickness\n"

    // squares([])
    //"float max_abs = max(abs(p.x), abs(p.y)); // Distance to farthest edge\n"
    //"float min_abs = min(abs(max_abs - 1.0), thickness); // Distance to edge boundary\n"
    //"if (max_abs <= 1.0 - thickness || min_abs >= thickness) discard; else\n"

    //"   if(dot(gl_PointCoord-0.5,gl_PointCoord-0.5)>0.25) discard; else\n" // circles 0
    //"   if(dot(p,p) > cos(theta*5)) discard; else\n" // roses *
    "   if(dot(p,p) > r || dot(p,p) < r*0.75) discard; else\n" // rings O
    //"   if(dot(p,p)> 5.0/cos(theta-20*r)) discard; else\n" // spirals @
    //"   if(dot(p,p) > 0.5*(exp(cos(theta*5)*0.75)) ) discard; else\n" // stars *
    "   fragcolor = vec4(out_color, 1.0);\n"
    "}";

uint64_t dd_state_apply(dd_flags_t *dst, const dd_flags_t *src) {
    if( dst->bits != src->bits ) {
        if( src->type ^ dst->type ) {
            glUseProgram(src->type ? dd_prg2 : dd_prg);
            glUniformMatrix4fv(src->type ? dd_prg2_mvp : dd_prg_mvp, 1, GL_FALSE, &dd_mvp.x.x);
        }
        if( src->color ^ dst->color || src->type ^ dst->type ) {
            unsigned rgba = src->color < 16 ? palette(src->color) : src->color | (255<<24);
            color_t c = { .bits = rgba };
            vec3 rgbf = { c.r / 255.f, c.g / 255.f, c.b / 255.f };
            glUniform3fv(src->type ? dd_prg2_color : dd_prg_color, GL_TRUE, &rgbf.x);
        }
        if( src->top ^ dst->top ) {
            (src->top ? glDisable : glEnable)(GL_DEPTH_TEST);
            glDepthMask(src->top ? GL_FALSE : GL_TRUE);
        }
#if 0
        if( src->aa ^ dst->aa ) {
            (src->aa ? glEnable : glDisable)(GL_MULTISAMPLE);
        }
#endif
        if( src->thin ^ dst->thin ) {
            const float line_widths[] = { 1, 0.625, 0.3, 0.15 };
            const float point_sizes[] = { 12, 10, 8, 4 };
            glPointSize(point_sizes[src->thin]);
            glLineWidth(line_widths[src->thin]);
        }

        *dst = *src;
    }
    uint64_t key = src->bits;
    return key;
}

unsigned dd_state_begin(mat44 proj, mat44 view) { // inits vao+vbo, flushes geometry, resets color
    ONCE {
    const char *vss[] = { preamble, dd_vs }; int vert = shader('v', 2, vss);
    const char *fss[] = { preamble, dd_fs_line_solid }; int frag = shader('f', 2, fss);
    dd_prg = program(2,vert,frag); // ,"att_position","fragcolor", NULL);
    dd_prg_color = glGetUniformLocation(dd_prg, "u_color");
    dd_prg_mvp = glGetUniformLocation(dd_prg, "u_MVP");

    const char *fss2[] = { preamble, dd_fs_point }; int frag2 = shader('f', 2, fss2);
    dd_prg2 = program(2,vert,frag2); // ,"att_position","fragcolor", NULL);
    dd_prg2_color = glGetUniformLocation(dd_prg2, "u_color");
    dd_prg2_mvp = glGetUniformLocation(dd_prg2, "u_MVP");
    }

    glActiveTexture(GL_TEXTURE0);

    dd_mvp = mat44_mul_mat44(view, proj), // MVP where M=id

    glUseProgram(dd_prg);
    glUniformMatrix4fv(dd_prg_mvp, 1, GL_FALSE, &dd_mvp.x.x);

    static GLuint vao, vbo;
    if(!vao) glGenVertexArrays(1, &vao);    glBindVertexArray(vao);
    if(!vbo) glGenBuffers(1, &vbo);         glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_CULL_FACE);

#if 1
    GLint depthFunc; glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
    int is_reversed_z = depthFunc == GL_GREATER || depthFunc == GL_GEQUAL;
    glDepthFunc(is_reversed_z ? GL_GREATER : GL_LEQUAL); // change < to <= and >= to >
#endif

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(1);
    glPointSize(1);

    GLboolean aa_restore = glIsEnabled(GL_MULTISAMPLE);
#if 0
    glDisable(GL_MULTISAMPLE);
#endif
    return aa_restore;
}

void dd_state_end(unsigned flags) {
    int aa_restore = flags;
#if 0
    (aa_restore ? glEnable : glDisable)(GL_MULTISAMPLE);
#endif

    glBindVertexArray(0);
    glUseProgram(0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glPointSize(1);
    glLineWidth(1);
}

void dd_commit(const void *bin, unsigned count, const dd_flags_t *hints) {
    // config vertex data
    glBufferData(GL_ARRAY_BUFFER, count * 3 * 4, bin, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // commit vertex data
    glDrawArrays(hints->type == 0 ? GL_LINES : GL_POINTS, 0, count);

    // profiling
    //perf_add("Render.drawcalls", 1);
    //perf_add("Render.vertices", count * (f.type ? 1 : 3));
}
