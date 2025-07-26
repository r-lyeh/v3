unsigned  program(int num_shaders, ...);
unsigned   shader(int type, int num_sources, const char **sources);
void      program_destroy(unsigned id);

unsigned  vao_begin();
unsigned   ebo_buffer(const char *fmt, const void *bin, unsigned len);
unsigned   vbo_buffer(int slot, const char *fmt, const void *bin, unsigned len);
void      vao_end();
void      vao_destroy(unsigned vao, int num, ...);

void      draw_triangles_uint(unsigned vao, unsigned tris, unsigned program);

#if CODE
#pragma once

unsigned shader(int type, int num_sources, const char **sources) {
    GLenum modes[] = {
        ['g'] = GL_GEOMETRY_SHADER, ['G'] = GL_GEOMETRY_SHADER, [GL_GEOMETRY_SHADER] = GL_GEOMETRY_SHADER,
        ['v'] = GL_VERTEX_SHADER,   ['V'] = GL_VERTEX_SHADER,   [GL_VERTEX_SHADER]   = GL_VERTEX_SHADER,
        ['f'] = GL_FRAGMENT_SHADER, ['F'] = GL_FRAGMENT_SHADER, [GL_FRAGMENT_SHADER] = GL_FRAGMENT_SHADER,
    };
    GLuint id = glCreateShader( modes[type] );
    glShaderSource(id, num_sources, sources, NULL);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if( !success ) {
        for( int src = 0; src < num_sources; ++src ) {
        int line = 0; for each_string(s, sources[src], "\r\n") printf("%d:%03d: %s\n", src, ++line, s);
        }

        GLchar infoLog[2048];
        glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
        die(va("error: shader compilation failed\n%s\n", infoLog));
    }
    return id;
}

unsigned program(int num_shaders, ...) {
    unsigned int prg = glCreateProgram();

    va_list v;
    va_start(v, num_shaders);
    for( int i = 0; i < num_shaders; ++i ) {
        unsigned shd = va_arg(v, unsigned);
        glAttachShader(prg, shd);
        glDeleteShader(shd);
    }
    va_end(v);

    GLint success;
    glLinkProgram(prg);
    glGetProgramiv(prg, GL_LINK_STATUS, &success);
    if( !success ) {
        GLchar infoLog[2048];
        glGetProgramInfoLog(prg, sizeof(infoLog), NULL, infoLog);
        die(va("error: shader compilation failed\n%s\n", infoLog));
    }

    return prg;
}
void program_destroy(unsigned id) {
    glDeleteProgram(id);
}

static
unsigned buffer(int slot, const char *fmt, const void *bin, unsigned len) {

    // format:
    // [!][!][Number]+[Type]
    //
    //     no leading  ! char denotes static buffer: no updates allowed
    // single leading  ! char denotes dynamic buffer: rarely updated
    // double leading !! char denotes stream buffer: updated every frame
    // [Number] denotes the amount of components in the vertex. non-digits prior to a number are discarded
    // [Type] can be [f]loat, [u]nsigned, [b]yte
    //        by default, bytes are normalised (0..255 as 0..1) and float/unsigned are not.
    //        type normalization can be flipped by using an uppercase letter.
    //
    // examples:
    // `2f2f4b` vec2 + vec2 + vec4b
    // `!!3f2f2i` vec3 + vec2 + vec2i (stream buffer)
    // `!vertex2f texcoord3f color4b` vec2 + vec3 + vec4b (dynamic buffer)
    // `2f2F` vec2 + vec2_norm

    enum {
        BUFFER_NO_UPDATES = GL_STATIC_DRAW,
        BUFFER_RARE_UPDATES = GL_DYNAMIC_DRAW,
        BUFFER_MANY_UPDATES = GL_STREAM_DRAW,
    };

    int update_mode = BUFFER_NO_UPDATES;
    if( fmt[0] == '!' ) update_mode = BUFFER_RARE_UPDATES, fmt++;
    if( fmt[0] == '!' ) update_mode = BUFFER_MANY_UPDATES, fmt++;

    int is_ebo = slot < 0;

    GLuint id;
    glGenBuffers(1, &id);

    GLenum mode = is_ebo ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
    glBindBuffer(mode, id);
    glBufferData(mode, len, bin, update_mode);

    if( !is_ebo ) { // is_vbo
        for( const char *chr = fmt; *chr; ++chr ) {
            if( isdigit(*chr) ) {
                int num = atoi(chr); while(isdigit(*chr)) ++chr;
                int types[] = { ['b'] = GL_UNSIGNED_BYTE, ['i'] = GL_UNSIGNED_INT, ['u'] = GL_UNSIGNED_INT, ['f'] = GL_FLOAT };
                int type = types[ tolower(*chr) ]; // strchr("bB", *chr) ? GL_UNSIGNED_BYTE : strchr("uiUI", *chr) ? GL_UNSIGNED_INT : GL_FLOAT;
                int norm = (type == GL_UNSIGNED_BYTE ? GL_TRUE : GL_FALSE) ^ (isupper(*chr) ? GL_TRUE : GL_FALSE);
                int size = type == GL_UNSIGNED_BYTE ? 1 : 4;
                glDisableVertexAttribArray(slot);
                glVertexAttribPointer(slot, num, type, norm, num * size, (void*)0);
                glEnableVertexAttribArray(slot);
            }
        }

        //if(...) die(va("buffer(): unknown vertex format `%s`", fmt));
    }

    return id;
}
static
void buffer_destroy(unsigned id) {
    glDeleteBuffers(1, &id);
}

unsigned vbo_buffer(int slot, const char *fmt, const void *bin, unsigned len) {
    return buffer(slot,fmt,bin,len);
}
unsigned ebo_buffer(const char *fmt, const void *bin, unsigned len) {
    return buffer(-1,fmt,bin,len);
}


unsigned vao_begin() {
    GLuint id = 0;
    glGenVertexArrays(1, &id);
    return glBindVertexArray(id), id;
}
void vao_end() {
    glBindVertexArray(0);
}
void vao_destroy(unsigned vao, int num, ...) {
    glDeleteVertexArrays(1, &vao);

    va_list v;
    va_start(v, num);
    for( int i = 0; i < num; ++i ) {
        unsigned id = va_arg(v, unsigned);
        glDeleteBuffers(1, &id);
    }
    va_end(v);
}

void draw_triangles_uint(unsigned vao, unsigned tris, unsigned program) {
    glUseProgram(program);
    glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, tris, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // safest, but likely unneeded
}

#endif

// GL version                     GLSL version
// 2.0                            1.10
// 2.1                            1.20
// 3.0                            1.30
// 3.1                            1.40
// 3.2                            1.50
// 3.3                            3.30
// 4.0                            4.00
// 4.1                            4.10
// 4.2                            4.20
// 4.3                            4.30
// 4.4                            4.40
// 4.5                            4.50
//
// GLES version                  GLSL version
// 2.0                            1.00 ES
// 3.0                            3.00 ES
// 3.1                            3.10 ES

#if 0
+-------------+----------------+------+-------------------------------------------------------------+
| GLSL Version| OpenGL Version | Year | Key Changes Against Previous Version                        |
+-------------+----------------+------+-------------------------------------------------------------+
| 1.10        | 2.0            | 2004 | Initial GLSL version. Programmable vertex/fragment shaders. |
|             |                |      | Basic types (float, vec2-vec4, mat2-mat4). Built-in vars    |
|             |                |      | (e.g., gl_FragColor, gl_Position). No geometry/tessellation.|
+-------------+----------------+------+-------------------------------------------------------------+
| 1.20        | 2.1            | 2006 | Non-square matrices (e.g., mat2x3, mat4x2). Added invariant |
|             |                |      | and centroid qualifiers. Improved texture lookups (e.g.,    |
|             |                |      | texture2D). Added preprocessor (#define, #ifdef).           |
+-------------+----------------+------+-------------------------------------------------------------+
| 1.30        | 3.0            | 2008 | Integer types (int, ivec2-ivec4, uint). Texture functions   |
|             |                |      | (e.g., texture2DRect). New functions (round, trunc, modf).  |
|             |                |      | Explicit attribute/varying locations. Deprecated some fixed-|
|             |                |      | function vars.                                              |
+-------------+----------------+------+-------------------------------------------------------------+
| 1.40        | 3.1            | 2009 | Removed fixed-function reliance. Added texture arrays       |
|             |                |      | (texture1DArray, texture2DArray). Uniform blocks. Enhanced  |
|             |                |      | integer ops and bit manipulation (e.g., bitfieldExtract).   |
+-------------+----------------+------+-------------------------------------------------------------+
| 1.50        | 3.2            | 2009 | Geometry shaders for per-primitive processing. Layout       |
|             |                |      | qualifiers for input/output locations. New vars (e.g.,      |
|             |                |      | gl_Layer, gl_PrimitiveID). Explicit uniform location binding|
+-------------+----------------+------+-------------------------------------------------------------+
| 3.30        | 3.3            | 2010 | Aligned with OpenGL 3.3 core. Explicit layout qualifiers for|
|             |                |      | vertex attributes (e.g., layout(location = 0)). Improved    |
|             |                |      | texture sampling (e.g., sampler2DRect). Interface blocks.   |
+-------------+----------------+------+-------------------------------------------------------------+
| 4.00        | 4.0            | 2010 | Tessellation shaders (control/evaluation). Double-precision |
|             |                |      | types (double, dvec2-dvec4, dmat2-dmat4). Tessellation vars |
|             |                |      | (e.g., gl_TessLevelInner). Shader subroutines.              |
+-------------+----------------+------+-------------------------------------------------------------+
| 4.10        | 4.1            | 2010 | Improved OpenGL ES compatibility. Viewport arrays for multi-|
|             |                |      | viewport rendering. Enhanced shader subroutines. Minor new  |
|             |                |      | functions and precision qualifiers.                         |
+-------------+----------------+------+-------------------------------------------------------------+
| 4.20        | 4.2            | 2011 | Atomic counters and image load/store (imageLoad,            |
|             |                |      | imageStore). Shader storage buffer objects (SSBOs).         |
|             |                |      | Explicit LOD texture sampling (e.g., textureLod).           |
+-------------+----------------+------+-------------------------------------------------------------+
| 4.30        | 4.3            | 2012 | Compute shaders for general computing. Shader storage blocks|
|             |                |      | for large data. Enhanced image load/store with format quals.|
|             |                |      | Debug output (gl_DebugMessageCallback). Improved bindings.  |
+-------------+----------------+------+-------------------------------------------------------------+
| 4.40        | 4.4            | 2013 | Buffer storage for persistent mapping. Texture gather ops   |
|             |                |      | (textureGather). Stricter layout rules for cross-vendor     |
|             |                |      | compatibility. Minor function additions.                    |
+-------------+----------------+------+-------------------------------------------------------------+
| 4.50        | 4.5            | 2014 | Direct state access (DSA) for shaders. Enhanced layout      |
|             |                |      | qualifiers for memory control. OpenGL ES 3.1 alignment. New |
|             |                |      | texture operation functions.                                |
+-------------+----------------+------+-------------------------------------------------------------+
| 4.60        | 4.6            | 2017 | SPIR-V support for binary shaders. Subgroup ops (e.g.,      |
|             |                |      | subgroupAdd, subgroupShuffle). Vulkan interoperability.     |
|             |                |      | Minor function/optimization additions.                      |
+-------------+----------------+------+-------------------------------------------------------------+
#endif
