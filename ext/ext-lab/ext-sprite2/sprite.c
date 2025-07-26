// # Original sprite renderer NEO by d7samurai [https://gist.github.com/d7samurai/e51adec8a440126d028b87406556079b]
// - Sprites are rendered back-to-front (AKA "painter's algorithm") in the order they are submitted,
//   as one draw call. The provided setup employs a single texture atlas containing all the sprite graphics.
// - The renderer is "immediate mode" in the sense that a fresh batch of sprite data (an array of spritedesc
//   entries specifying what textures to use, their position, scale, rotation etc) is uploaded every frame.
//   The texture data itself is retained on the GPU.
// - ARGB filtering & alpha blending.
// - Custom antialiased point sampling that works for smooth sub-pixel movement, non-integer scaling and arbitrary rotation.
// - Sprite textures must be separated by at least a 1-pixel wide, fully transparent gap (0x00000000):
// - Anchor/pivot point: (0.0, 0.0) is top-left, (1.0, 1.0) is bottom-right, but can be set to anywhere inside or outside sprite.
// - The sprite's position corresponds to the position of the anchor point on screen, with the
//   sprite texture aligned accordingly. Similarly, rotation and scale (including horizontal and
//   vertical mirroring) are done around the anchor (pivot) point.
// - The antialiasing technique used in this renderer requires premultiplied alpha textures.
// - Sprite sheet uses characters from Tiny Heroes @ CraftPix.net.

#define V3_IMPLEMENTATION
#include "v3.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "spritesheet.h"

#define MAX_SPRITES "1024"

const char* sprite_vs =
    "#version 330 core\n"

    "struct SpriteDesc {\n"
    "    vec2 location;\n"
    "    vec2 size;\n"
    "    vec2 anchor;\n"
    "    vec2 position;\n"
    "    vec2 scale;\n"
    "    float rotation;\n"
    "    uint color;\n"
    "};\n"

    "layout(std140) uniform Constants {\n"
    "    vec2 rn_screensize;\n"
    "    vec2 r_texturesize;\n"
    "};\n"

    "layout(std140) uniform SpriteBuffer {\n"
    "    SpriteDesc sprites[" MAX_SPRITES "];\n"
    "};\n"

    "out vec2 v_location;\n"
    "out vec4 v_color;\n"

    "void main() {\n"
    "    uint spriteid = uint(gl_InstanceID);\n"
    "    uint vertexid = uint(gl_VertexID);\n"
        "\n"
    "    SpriteDesc sprite = sprites[spriteid];\n"
        "\n"
    "    uvec2 idx = uvec2(vertexid & 2u, ((vertexid << 1u) & 2u) ^ 3u);\n"
        "\n"
    "    vec4 piv = vec4(0.0, 0.0, sprite.size + 1.0) * sprite.scale.xyxy \n"
    "               - (sprite.size * sprite.scale * sprite.anchor).xyxy;\n"
        "\n"
    "    float cosRot = cos(sprite.rotation);\n"
    "    float sinRot = sin(sprite.rotation);\n"
    "    vec2 pos = vec2(\n"
    "        piv[idx.x] * cosRot - piv[idx.y] * sinRot,\n"
    "        piv[idx.y] * cosRot + piv[idx.x] * sinRot\n"
    "    ) + sprite.position - 0.5;\n"
        "\n"
    "    vec4 loc = vec4(sprite.location, sprite.location + sprite.size + 1.0);\n"
        "\n"
    "    gl_Position = vec4(pos * rn_screensize - vec2(1.0, -1.0), 0.0, 1.0);\n"
    "    v_location = vec2(loc[idx.x], loc[idx.y]);\n"
    "    v_color = vec4(\n"
    "        float((sprite.color >> 16u) & 0xffu),\n"
    "        float((sprite.color >> 8u) & 0xffu),\n"
    "        float((sprite.color >> 0u) & 0xffu),\n"
    "        float((sprite.color >> 24u) & 0xffu)\n"
    "    ) / 255.0;\n"
    "}\n";

const char* sprite_fs =
    "#version 330 core\n"
    "\n"
    "layout(std140) uniform Constants {\n"
    "    vec2 rn_screensize;\n"
    "    vec2 r_texturesize;\n"
    "};\n"
    "\n"
    "uniform sampler2D spritesheet;\n"
    "\n"
    "in vec2 v_location;\n"
    "in vec4 v_color;\n"
    "\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main() {\n"
    "    vec2 texCoord = (floor(v_location) + \n"
    "                     min(fract(v_location) / fwidth(v_location), 1.0) - 0.5) \n"
    "                    * r_texturesize;\n"
    "\n"
    "    vec4 color = texture(spritesheet, texCoord) * v_color.a * vec4(v_color.rgb, 1.0);\n"
    "\n"
    "    if (color.a == 0.0) discard;\n"
    "\n"
    "    fragColor = color;\n"
    "}\n";


int main() {
    app_create();

    // Compile shaders
    GLuint shaderProgram = program(2, shader('v', 1, &sprite_vs), shader('f', 1, &sprite_fs));

    // Create uniform buffer for constants
    float constants[4] = { 
        2.0f / app_width(), 
        -2.0f / app_height(), 
        1.0f / (float)SPRITESHEET_WIDTH, 
        1.0f / (float)SPRITESHEET_HEIGHT 
    };

    GLuint constantBuffer;
    glGenBuffers(1, &constantBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, constantBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(constants), constants, GL_STATIC_DRAW);

        // Bind uniform block to binding point 0
        GLuint constantsBlockIndex = glGetUniformBlockIndex(shaderProgram, "Constants");
        glUniformBlockBinding(shaderProgram, constantsBlockIndex, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, constantBuffer);

    // Create sprite buffer

    typedef struct spritedesc { vec2 location, size, anchor, position, scale; float rotation; int color; } spritedesc;

    GLuint spriteBuffer;
    glGenBuffers(1, &spriteBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, spriteBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(spritedesc) * atoi(MAX_SPRITES), NULL, GL_DYNAMIC_DRAW);

        // Bind uniform block to binding point 1
        GLuint spriteBlockIndex = glGetUniformBlockIndex(shaderProgram, "SpriteBuffer");
        glUniformBlockBinding(shaderProgram, spriteBlockIndex, 1);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, spriteBuffer);

    spritedesc* spritebatch = realloc(0, sizeof(spritedesc) * atoi(MAX_SPRITES));

    // Create texture (spritesheet)
    GLuint spriteTexture;
    glGenTextures(1, &spriteTexture);
    glBindTexture(GL_TEXTURE_2D, spriteTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, SPRITESHEET_WIDTH, SPRITESHEET_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, spritesheetdata);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Create VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Setup render state
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    GLint spritesheetLoc = glGetUniformLocation(shaderProgram, "spritesheet");

    // Main loop
    int tick = 0;
    float rotation = 0.0f;

    const int count = argc() > 1 ? atoi(argv(2)) : 1;

    while (app_swap())
    {
#if 1
        // Clear
        glViewport(0, 0, app_width(), app_height());
        glClearColor(0.1337f, 0.1337f, 0.1337f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
#endif

        int spritecount = 0;

        int frame = tick++ / 6 % ANIMATION_FRAMES;
        vec2 cell = { (float)(frame % SPRITESHEET_COLUMNS), (float)(frame / SPRITESHEET_COLUMNS) };

        spritedesc sprite;
        sprite.location = vec2( cell.x * CELL_WIDTH, cell.y * CELL_HEIGHT );
        sprite.size     = vec2( SPRITE_WIDTH, SPRITE_HEIGHT );
        sprite.anchor   = vec2( 0.5f, 2.0f );
        sprite.position = vec2( app_width() * 0.5f, app_height() * 0.5f );
        sprite.scale    = vec2( 4.0f, 4.0f );
        sprite.rotation = rotation += 0.00873f;
        sprite.color    = 0xffffffff;

        spritebatch[spritecount++] = sprite;

        // Upload sprite batch
        glBindBuffer(GL_UNIFORM_BUFFER, spriteBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(spritedesc) * spritecount, spritebatch);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, spriteTexture);

        glUseProgram(shaderProgram);
        glUniform1i(spritesheetLoc, 0);

        // draw
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, spritecount);

        ui_label(va("%d sprites, %5.2ffps", spritecount, fps()));
    }

    return 0;
}
