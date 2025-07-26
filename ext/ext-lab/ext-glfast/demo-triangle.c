#define V3_IMPLEMENTATION
#include "v3.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#endif

#define fopen(f,m) fopen(lookup(f),m)

#ifndef GLFAST_IMPLEMENTATION
#define GLFAST_IMPLEMENTATION
#include "glfast.h"
#endif

i32 main()
{
  app_create(.title = "glFast(triangle)", .scale = 0.75, .extra = "srgb");
  
  gpu_storage_t mesh = {0};
  mesh.format = xyzw_f32;
  mesh.count  = 3;
  mesh = gfStorageCreateFromStruct(mesh);
  
  mesh.as_vec4[0].x = -0.5f;
  mesh.as_vec4[0].y = -0.5f;
  mesh.as_vec4[0].z =  0.0f;
  mesh.as_vec4[0].w =  0.0f;
  
  mesh.as_vec4[1].x =  0.0f;
  mesh.as_vec4[1].y =  0.5f;
  mesh.as_vec4[1].z =  0.0f;
  mesh.as_vec4[1].w =  0.0f;
  
  mesh.as_vec4[2].x =  0.5f;
  mesh.as_vec4[2].y = -0.5f;
  mesh.as_vec4[2].z =  0.0f;
  mesh.as_vec4[2].w =  0.0f;
  
  gpu_cmd_t cmd[1] = {0};
  
  cmd[0].count = mesh.count;
  cmd[0].instance_count = 1;
  
  const char * vs_str = VERT_HEAD
    "layout(std430, binding = 0) readonly buffer in_pos_ { vec4 in_pos[]; };\n"
    "void main() {\n"
    "  vec3 pos = in_pos[gl_VertexID].xyz;\n"
    "  gl_Position = vec4(pos, 1.f);\n"
    "}\n";
  
  const char * fs_str = FRAG_HEAD
    "out vec4 color;\n"
    "void main() {\n"
    "  color = vec4(1.f);\n"
    "}\n";

  u32 vs = gfProgramCreateFromString(GL_VERTEX_SHADER, vs_str);
  u32 fs = gfProgramCreateFromString(GL_FRAGMENT_SHADER, fs_str);
  u32 pp = gfProgramPipelineCreate(vs, fs);
  
  u32 vertex_buffers[1] = { mesh.id };

  for( int i = 0; i < 1; ++i)
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, vertex_buffers[i]);
  
  while(app_swap())
  {
    gfClear();
    gfDraw(pp, COUNTOF(cmd), cmd);
  }
  
  return 0;
}
