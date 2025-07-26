#if 0
## renderer architecture
- [ ] there are as many renderqueues as running threads
- [ ] each thread pushes rendercmds into its local renderqueue
- [ ] each thread will eventually:
  - [ ] collect all visible objects(): level sectors > spatial culling > frustum culling > occlusion culling etc.
  - [x] sort all visible objects(): transparent and overlay objects are sorted differently.
  - [ ] submit processed renderqueue() to rendering system
- [ ] filtered queues are flattened into a big list, which is processed as follows:
  - [x] `foreach( pipeline state ) set(pipeline) +`
  - [ ] `foreach( rendertarget ) set(rendertarget) +`
  - [ ] `foreach( pass ) set(pass) +`
  - [ ] `foreach( material ) set(shader+textures) +`
  - [ ] `foreach( material uniforms ) set(uniforms+textures) +`
  - [x] `foreach( vertex format ) set(vao) +`
  - [x] `foreach( object ) glDrawElementsInstancedBaseVertex(GL_TRIANGLES, ...)`

## notes
- collect: ensure this happens per-thread to distribute load. For occlusion, use hardware occlusion queries (glBeginQuery(GL_SAMPLES_PASSED, query)) to avoid sorting culled objects.
- sort: parallel_radix_sort
- pipeline: For modern OpenGL (4.1+), cache state changes (e.g., blend func, depth func, wireframe...) as PSOs to avoid redundant glEnable/glDisable calls, which can be expensive on some drivers.
- object: For repeated objects (e.g., foliage), add instanced rendering (glDrawElementsInstancedBaseVertex) to the foreach(object) loop. Group instances by material/vertex format in the sort key to batch them.
  
## references
- https://nlguillemot.wordpress.com/2016/11/18/opengl-renderer-design/
#endif

#define V3_IMPLEMENTATION
#include "v3.h"

// Sort key struct with bitfields, totaling 64 bits
typedef union {
    struct {
        uint64_t renderState : 2;       // 0=opaque, 1=alpha cutoff, 2=transparency, 3=overlay
        uint64_t renderTarget : 6;      // Render target ID (up to 64)
        uint64_t pass : 5;              // Render pass ID (up to 32)
        uint64_t material : 12;         // Material ID (up to 4096)
        uint64_t materialInstance : 10; // Material instance ID (up to 1024)
        uint64_t vertexFormat : 4;      // Vertex format ID (up to 16)
        uint64_t depth : 24;            // Depth value (inverted for transparency and overlay)
    };
    struct {
        uint64_t state : 40;
    };
    uint64_t bits;
} SortKey_t;

// Ensure SortKey_t is 64 bits long
STATIC_ASSERT(sizeof(SortKey_t) == 8);

typedef struct {
    float x, y, z;  // Instance position offset
} InstanceData;

typedef struct {
    SortKey_t sortKey;           // 64-bit sort key
    float depth;                 // Distance to camera
    void* indexDataOffset;       // Byte offset into index buffer
    int indexCount;              // Number of indices
    //int baseVertex;              // Base vertex for glDrawElementsBaseVertex
    int instanceCount;           // Number of instances to render
    void* instanceDataOffset;    // Byte offset into instance buffer
} RenderItem;

// Generate SortKey_t from RenderItem
static SortKey_t GenerateSortKey(const RenderItem* item) {
    SortKey_t key = item->sortKey;
    uint32_t depthInt = (uint32_t)(item->depth / 1000.0f * 0xFFFFFF); // Max depth 1000 units
    if (key.renderState == 2 || key.renderState == 3) {
        depthInt = 0xFFFFFF - depthInt; // Invert depth for transparency and overlay
    }
    key.depth = depthInt & 0xFFFFFF;
    return key;
}

// Comparison function for qsort
static int CompareRenderItems(const void* a, const void* b) {
    const RenderItem* itemA = (const RenderItem*)a;
    const RenderItem* itemB = (const RenderItem*)b;
    uint64_t keyA = *(uint64_t*)&itemA->sortKey;
    uint64_t keyB = *(uint64_t*)&itemB->sortKey;
    return (keyA < keyB) ? -1 : (keyA > keyB) ? 1 : 0;
}

// Set up render queue state
static void setupRenderState(uint32_t renderState, GLuint program) {
    // Use uniform to control alpha cutoff in shader
    GLint alphaCutoffLoc = glGetUniformLocation(program, "uAlphaCutoff");
    if (renderState == 0) { // Opaque
        glUniform1f(alphaCutoffLoc, 0.0f); // Disable cutoff
        glDisable(GL_BLEND);
#if 1
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);     // Write depth
#endif
    } else if (renderState == 1) { // Alpha cutoff
        glUniform1f(alphaCutoffLoc, 0.5f); // Enable cutoff in shader
        glDisable(GL_BLEND);
#if 1
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);  // Write depth for opaque-like behavior
#endif
    } else if (renderState == 2) { // Transparency
        glUniform1f(alphaCutoffLoc, 0.0f); // Disable cutoff
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#if 1
        glDisable(GL_DEPTH_TEST); // Disable depth test to ensure all transparent fragments render, relying on back-to-front sorting
        glDepthMask(GL_FALSE);    // Prevent depth writes (optional, for consistency)
#endif
    } else if (renderState == 3) { // Overlay
        glUniform1f(alphaCutoffLoc, 0.0f); // Disable cutoff
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#if 1
        glDisable(GL_DEPTH_TEST); // Ignore depth entirely for UI
#endif
    } else { // clean up
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
    }
}

// Write uniform data (placeholder, using vertex colors)
static void writeUniformData(const RenderItem* item) {
    // Relies on vertex colors for this demo
}

// Shader sources
const char* bucket_vs = ifdef(V3_MACOS, "#version 150\n", "#version 130\n")
    /*"layout(location = 0)*/" in vec3 aPos;\n"
    /*"layout(location = 1)*/" in vec4 aColor;\n"
    /*"layout(location = 2)*/" in vec3 aInstancePos;\n" // Per-instance position offset
    "out vec4 vColor;\n"
    "void main() {\n"
    "    vec3 pos = aPos + aInstancePos; // Offset position by instance data\n"
    "    gl_Position = vec4(pos, 1.0);\n"
    "    vColor = aColor;\n"
    "}\n";

const char* bucket_fs = ifdef(V3_MACOS, "#version 150\n", "#version 130\n")
    "in vec4 vColor;\n"
    "out vec4 FragColor;\n"
    "uniform float uAlphaCutoff = 0.0;\n"
    "void main() {\n"
    "    if (vColor.a < uAlphaCutoff) discard;\n"
    "    FragColor = vColor;\n"
    "}\n";

static GLuint compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        die(va("shader error: %s\n", infoLog));
    }
    return shader;
}

static GLuint createProgram(const char *attribs) {
    GLuint vertexShader = compileShader(bucket_vs, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(bucket_fs, GL_FRAGMENT_SHADER);
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // @todo
    // scan vertex shader instead: "*in vec* %s;" 
    int attrib_slot = 0;
    if( attribs && attribs[0] )
    for each_string(attrib_name,attribs,";,") {
        glBindAttribLocation(program, attrib_slot++, attrib_name);
    }

    glLinkProgram(program);
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        exit(1);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

void RenderScene(RenderItem* renderItems, int itemCount, GLuint vao, GLuint ibo, GLuint program) {
    // Bind shader program
    glUseProgram(program);

    // Bind VAO and IBO
    glBindVertexArray(vao);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Generate sort keys
    for (int i = 0; i < itemCount; i++) {
        renderItems[i].sortKey = GenerateSortKey(&renderItems[i]);
    }

    uint64_t ns1 = time_ns();

    // Sort render items
    qsort(renderItems, itemCount, sizeof(RenderItem), CompareRenderItems);

    uint64_t ns2 = time_ns();

    // Render
    SortKey_t current = { .state = -1 };
    for (int i = 0; i < itemCount; i++) {
        const RenderItem* item = &renderItems[i];
        const SortKey_t sort = item->sortKey;

        if (current.state != sort.state) {
            // Set render queue state; aka GL pipelines
            if (sort.renderState != current.renderState) {
                setupRenderState(sort.renderState, program);
                current.renderState = sort.renderState;
            }

            // Bind render target (placeholder); aka render-to-texture
            if (sort.renderTarget != current.renderTarget) {
                // e.g., glBindFramebuffer(GL_FRAMEBUFFER, sort.renderTarget); + error checking
                current.renderTarget = sort.renderTarget;
            }

            // Set pass state (placeholder); aka camera pass,depth pass,etc
            if (sort.pass != current.pass) {
                current.pass = sort.pass;
            }

            // Bind material (placeholder); aka shaders+textures
            if (sort.material != current.material) {
                current.material = sort.material;
            }

            // Bind material instance (placeholder); aka uniforms (material properties)
            if (sort.materialInstance != current.materialInstance) {
                current.materialInstance = sort.materialInstance;
                writeUniformData(item); // UBOs+program
            }

            // Rebind VAO/IBO if vertex format changes
            if (sort.vertexFormat != current.vertexFormat) {
                glBindVertexArray(vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
                current.vertexFormat = sort.vertexFormat;
            }
        }

        // Draw the object
        glDrawElementsInstancedBaseVertex(
            GL_TRIANGLES,
            item->indexCount,
            GL_UNSIGNED_SHORT,
            item->indexDataOffset,
            item->instanceCount + !item->instanceCount, // always 1 or N, but never 0
            0 //item->baseVertex
        );
    }

    setupRenderState(~0u, program); // cleanup

    // Unbind VAO and IBO
    glBindVertexArray(0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    uint64_t ns3 = time_ns();

    ui_label(va("sort: %5.2fms draw: %5.2fms", (ns2-ns1)/1e6, (ns3-ns2)/1e6));
}

int main() {
    app_create(.title = "v3(render4)", .scale = 0.75);

    // Set up OpenGL resources
    GLuint program = createProgram("aPos,aColor,aInstancePos");

    // Vertex data: 14 vertices for 2 triangles and 2 quads
    typedef struct {
        float x, y, z;
        float r, g, b, a;
    } Vertex;
    Vertex vertices[] = {
        // Triangle 1 (opaque, red)
        {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}, // v0
        { 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}, // v1
        { 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f}, // v2
        // Quad (alpha cutoff, green)
        {-0.3f, -0.3f, 0.1f, 0.0f, 1.0f, 0.0f, 1.0f}, // v3
        { 0.3f, -0.3f, 0.1f, 0.0f, 1.0f, 0.0f, 1.0f}, // v4
        { 0.3f,  0.3f, 0.1f, 0.0f, 1.0f, 0.0f, 1.0f}, // v5
        {-0.3f,  0.3f, 0.1f, 0.0f, 1.0f, 0.0f, 1.0f}, // v6
        // Triangle 2 (transparent, blue)
        {-0.4f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f, 0.5f}, // v7
        { 0.0f,  0.4f, 0.2f, 0.0f, 0.0f, 1.0f, 0.5f}, // v8
        { 0.4f, -0.4f, 0.2f, 0.0f, 0.0f, 1.0f, 0.5f}, // v9
        // Quad 2 (overlay, yellow)
        {-0.2f, -0.2f, 0.3f, 1.0f, 1.0f, 0.0f, 0.5f}, // v10
        { 0.2f, -0.2f, 0.3f, 1.0f, 1.0f, 0.0f, 0.5f}, // v11
        { 0.2f,  0.2f, 0.3f, 1.0f, 1.0f, 0.0f, 0.5f}, // v12
        {-0.2f,  0.2f, 0.3f, 1.0f, 1.0f, 0.0f, 0.5f}, // v13
    };

    // Index data
    GLushort indices[] = {
        0, 1, 2,          // Opaque triangle
        3, 4, 5, 5, 6, 3, // Alpha cutoff quad
        7, 8, 9,          // Transparent triangle
        10, 11, 12, 12, 13, 10, // Overlay quad
    };

    // Instance data (e.g., 4 instances of the opaque triangle)
    InstanceData instances[] = {
        {-0.5f, 0.0f, 0.0f}, // Instance 1
        { 0.5f, 0.0f, 0.0f}, // Instance 2
        { 0.0f, 0.5f, 0.0f}, // Instance 3
        { 0.0f,-0.5f, 0.0f}  // Instance 4
    };

    // Set up VBO and IBO
    GLuint ibo, vbo, instanceVbo;
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &instanceVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(instances), instances, GL_STATIC_DRAW);

    // Set up VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0); // aPos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1); // aColor
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
    glEnableVertexAttribArray(2); // aInstancePos
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)0);
    glVertexAttribDivisor(2, 1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBindVertexArray(0);

    // Render items with corrected index offsets and instance data
    RenderItem items[5] = {
        {{.renderState=0, .material=0}, 100.0f, (void*)( 0 * sizeof(GLushort)), 3, 1, NULL}, // Opaque triangle (non-instanced)
        {{.renderState=1, .material=1}, 200.0f, (void*)( 3 * sizeof(GLushort)), 6, 1, NULL}, // Alpha cutoff quad (non-instanced)
        {{.renderState=2, .material=2}, 300.0f, (void*)( 9 * sizeof(GLushort)), 3, 1, NULL}, // Transparent triangle (non-instanced)
        {{.renderState=3, .material=3}, 400.0f, (void*)(12 * sizeof(GLushort)), 6, 1, NULL}, // Overlay quad (non-instanced)
        {{.renderState=0, .material=1}, 150.0f, (void*)( 0 * sizeof(GLushort)), 3, 4, NULL}, // Instanced opaque triangle (4 instances)
    };

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (app_swap()) {
        ui_label(va("%dx%d", app_width(),app_height()));
#if 0   // app_swap() does this
        glViewport(0,0,app_width(),app_height());
        glScissor(0,0,app_width(),app_height());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
        if( ui_button("Toggle MSAA") ) { int on = glIsEnabled(GL_MULTISAMPLE); (on?glDisable:glEnable)(GL_MULTISAMPLE); }
        RenderScene(items, COUNTOF(items), vao, ibo, program);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &instanceVbo);
    glDeleteProgram(program);

    return 0;
}
