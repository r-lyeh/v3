// Updated glfast.h with migration to SSBO instead of TBO.
// Minimal changes: Removed TBO creation (glCreateTextures and glTextureBuffer).
// Set storage.id = storage.buffer_id.
// Format is kept but no longer used for glTextureBuffer.
// Note: Shaders must be updated to use SSBO syntax (see below).
// Demo binding must change to use glBindBufferBase (see demo updates below).

// Public domain single-file C implementation by Constantine Tarasenkov
//
// LICENSE
//
// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

#ifndef GLFAST_H_
#define GLFAST_H_

// GLFAST //////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// RUST TYPES //////////////////////////////////////////////////////////////////

#ifndef i8
#define i8 int8_t
#endif

#ifndef i16
#define i16 int16_t
#endif

#ifndef i32
#define i32 int32_t
#endif

#ifndef i64
#define i64 int64_t
#endif

#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif

#ifndef u64
#define u64 uint64_t
#endif

#ifndef f16
#define f16 uint16_t
#endif

#ifndef f32
#define f32 float
#endif

#ifndef f64
#define f64 double
#endif

// HELPERS /////////////////////////////////////////////////////////////////////

#define x_b8      GL_R8
#define x_f16     GL_R16F
#define x_f32     GL_R32F
#define x_i8      GL_R8I
#define x_i16     GL_R16I
#define x_i32     GL_R32I
#define x_u8      GL_R8UI
#define x_u16     GL_R16UI
#define x_u32     GL_R32UI
#define xy_b8     GL_RG8
#define xy_f16    GL_RG16F
#define xy_f32    GL_RG32F
#define xy_i8     GL_RG8I
#define xy_i16    GL_RG16I
#define xy_i32    GL_RG32I
#define xy_u8     GL_RG8UI
#define xy_u16    GL_RG16UI
#define xy_u32    GL_RG32UI
#define xyzw_b8   GL_RGBA8
#define xyzw_f16  GL_RGBA16F
#define xyzw_f32  GL_RGBA32F
#define xyzw_i8   GL_RGBA8I
#define xyzw_i16  GL_RGBA16I
#define xyzw_i32  GL_RGBA32I
#define xyzw_u8   GL_RGBA8UI
#define xyzw_u16  GL_RGBA16UI
#define xyzw_u32  GL_RGBA32UI

#define rgb_b8    GL_RGB8
#define rgba_b8   GL_RGBA8
#define srgb_b8   GL_SRGB8
#define srgba_b8  GL_SRGB8_ALPHA8
#define rgba_f16  GL_RGBA16F
#define rgba_f32  GL_RGBA32F
#define depth_b16 GL_DEPTH_COMPONENT16
#define depth_b24 GL_DEPTH_COMPONENT24
#define depth_b32 GL_DEPTH_COMPONENT32

#define VERT_HEAD \
  "#version 430\n" \
  "#extension GL_ARB_shader_precision          : enable\n" \
  "#extension GL_ARB_shading_language_420pack  : enable\n" \
  "#extension GL_ARB_explicit_uniform_location : enable\n" \
  "out gl_PerVertex { vec4 gl_Position; };\n"

#define FRAG_HEAD \
  "#version 330\n" \
  "#extension GL_ARB_shader_precision          : enable\n" \
  "#extension GL_ARB_conservative_depth        : enable\n" \
  "#extension GL_ARB_texture_cube_map_array    : enable\n" \
  "#extension GL_ARB_shading_language_420pack  : enable\n" \
  "#extension GL_ARB_shading_language_packing  : enable\n" \
  "#extension GL_ARB_explicit_uniform_location : enable\n" \
  "layout(depth_unchanged) out float gl_FragDepth;\n"

// TYPES ///////////////////////////////////////////////////////////////////////

typedef struct
{
  union
  {
    u8   * ptr;
    i8   * as_i8;
    u8   * as_u8;
    i16  * as_i16;
    u16  * as_u16;
    u16  * as_uint16;
    i32  * as_i32;
    u32  * as_u32;
    u32  * as_uint32;
    f32  * as_f32;
    struct
    {
      union { f32 x; f32 u; };
      union { f32 y; f32 v; };
    } * as_vec2;
    struct
    {
      union { f32 x; f32 r; };
      union { f32 y; f32 g; };
      union { f32 z; f32 b; };
      union { f32 w; f32 a; };
    } * as_vec4;
  };
  u32 bytes;
  u32 count;
  u32 format;
  u32 buffer_id;
  u32 id;
} gpu_storage_t;

typedef struct
{
  i32 w;
  i32 h;
  i32 count;
  u32 format;
  i32 mipmap;
  u8  cubemap;
  u32 id;
} gpu_texture_t;

typedef struct
{
  i32 aniso;
  i32 min;
  i32 mag;
  i32 wrap;
  u32 id;
} gpu_sampler_t;

typedef struct
{
  u32 count;
  u32 instance_count;
  u32 first;
  u32 instance_first;
} gpu_cmd_t;

// PROTOTYPES //////////////////////////////////////////////////////////////////

#ifndef __cplusplus
#define gfStorageCreate(...) gfStorageCreateFromStruct((gpu_storage_t){__VA_ARGS__})
#define gfTextureCreate(...) gfTextureCreateFromStruct((gpu_texture_t){__VA_ARGS__})
#define gfSamplerCreate(...) gfSamplerCreateFromStruct((gpu_sampler_t){__VA_ARGS__})
#else
#define gfStorageCreate(...) gfStorageCreateFromStruct( gpu_storage_t {__VA_ARGS__})
#define gfTextureCreate(...) gfTextureCreateFromStruct( gpu_texture_t {__VA_ARGS__})
#define gfSamplerCreate(...) gfSamplerCreateFromStruct( gpu_sampler_t {__VA_ARGS__})
#endif

void gfInit(void);
gpu_storage_t gfStorageCreateFromStruct(gpu_storage_t tbo);
gpu_texture_t gfTextureCreateFromStruct(gpu_texture_t texture);
gpu_sampler_t gfSamplerCreateFromStruct(gpu_sampler_t sampler);
gpu_texture_t gfTextureCreateFromFile(i32 width, i32 height, i32 mipmap, i32 texture_count, const char ** texture_paths);
gpu_texture_t gfCubemapCreateFromFile(i32 width, i32 height, i32 mipmap, i32 texture_count, const char ** pos_x_texture_paths, const char ** neg_x_texture_paths, const char ** pos_y_texture_paths, const char ** neg_y_texture_paths, const char ** pos_z_texture_paths, const char ** neg_z_texture_paths);
void gfTextureSetPixels(u32 texture_id, i32 texture_layer, i32 x, i32 y, i32 width, i32 height, u32 format, const void * data);
void gfTextureGetPixels(u32 texture_id, i32 texture_layer, i32 x, i32 y, i32 width, i32 height, u32 format, i32 pixels_bytes, void * pixels);
void gfTextureSaveToFile(u32 texture_id, i32 texture_layer, i32 width, i32 height, const char * pathfile);
u32 gfFboCreate(u32 depth_texture_id, i32 depth_texture_layer, i32 color_texture_count, const u32 * color_texture_ids, const i32 * color_texture_layers);
void gfFboBind(u32 fbo_id);
u32 gfProgramCreateFromFile(u32 shader_type, const char * shader_filepath);
u32 gfProgramCreateFromString(u32 shader_type, const char * shader_string);
u32 gfProgramPipelineCreate(u32 vert_program, u32 frag_program);
void gfClear();
void gfDraw(u32 program_pipeline, u32 gpu_cmd_count, const gpu_cmd_t * gpu_cmd);
void gfFire(u32 program_pipeline, u32 count);
void gfDebugCallback(u32 source, u32 type, u32 id, u32 severity, i32 length, const char * message, void * userdata);
bool gfCheckExtensions(unsigned glVersion, unsigned extensions_count, const char ** extensions);
void gfError(const char * title, const char * description);

#ifdef __cplusplus
}
#endif
#endif // GLFAST_H_

// IMPLEMENTATION //////////////////////////////////////////////////////////////

#ifdef GLFAST_IMPLEMENTATION

unsigned gfFormatBytes(unsigned storage_format)
{
  u32 elem_width = 0;
  u32 elem_bytes = 0;

  switch(storage_format)
  {
    case(x_b8):     elem_width = 1; elem_bytes = sizeof(u8);  break;
    case(x_f16):    elem_width = 1; elem_bytes = sizeof(f16); break;
    case(x_f32):    elem_width = 1; elem_bytes = sizeof(f32); break;
    case(x_i8):     elem_width = 1; elem_bytes = sizeof(i8);  break;
    case(x_i16):    elem_width = 1; elem_bytes = sizeof(i16); break;
    case(x_i32):    elem_width = 1; elem_bytes = sizeof(i32); break;
    case(x_u8):     elem_width = 1; elem_bytes = sizeof(u8);  break;
    case(x_u16):    elem_width = 1; elem_bytes = sizeof(u16); break;
    case(x_u32):    elem_width = 1; elem_bytes = sizeof(u32); break;
    case(xy_b8):    elem_width = 2; elem_bytes = sizeof(u8);  break;
    case(xy_f16):   elem_width = 2; elem_bytes = sizeof(f16); break;
    case(xy_f32):   elem_width = 2; elem_bytes = sizeof(f32); break;
    case(xy_i8):    elem_width = 2; elem_bytes = sizeof(i8);  break;
    case(xy_i16):   elem_width = 2; elem_bytes = sizeof(i16); break;
    case(xy_i32):   elem_width = 2; elem_bytes = sizeof(i32); break;
    case(xy_u8):    elem_width = 2; elem_bytes = sizeof(u8);  break;
    case(xy_u16):   elem_width = 2; elem_bytes = sizeof(u16); break;
    case(xy_u32):   elem_width = 2; elem_bytes = sizeof(u32); break;
    case(xyzw_b8):  elem_width = 4; elem_bytes = sizeof(u8);  break;
    case(xyzw_f16): elem_width = 4; elem_bytes = sizeof(f16); break;
    case(xyzw_f32): elem_width = 4; elem_bytes = sizeof(f32); break;
    case(xyzw_i8):  elem_width = 4; elem_bytes = sizeof(i8);  break;
    case(xyzw_i16): elem_width = 4; elem_bytes = sizeof(i16); break;
    case(xyzw_i32): elem_width = 4; elem_bytes = sizeof(i32); break;
    case(xyzw_u8):  elem_width = 4; elem_bytes = sizeof(u8);  break;
    case(xyzw_u16): elem_width = 4; elem_bytes = sizeof(u16); break;
    case(xyzw_u32): elem_width = 4; elem_bytes = sizeof(u32); break;
    default: gfError(__FUNCTION__, "Error: Wrong Storage format");
  }

  return elem_width * elem_bytes;
}

gpu_storage_t gfStorageCreateFromStruct(gpu_storage_t storage)
{
  u32 buffer_id;
  glCreateBuffers(1, &buffer_id);
  
  void *ptr = storage.ptr;
  u32 bytes = storage.bytes ? storage.bytes : storage.count * gfFormatBytes(storage.format);
  
  if(ptr)
    glNamedBufferStorage(buffer_id, bytes, ptr, GL_DYNAMIC_STORAGE_BIT);
  else
  {
    unsigned flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT; // | GL_MAP_COHERENT_BIT);
    glNamedBufferStorage(buffer_id, bytes, NULL, flags);
    storage.ptr = glMapNamedBufferRange(buffer_id, 0, bytes, flags);
  }
  
  storage.buffer_id = buffer_id;
  storage.id = buffer_id;  // Changed: Use buffer_id directly for SSBO
  
  return storage;
}

gpu_texture_t gfTextureCreateFromStruct(gpu_texture_t texture)
{
  if(!texture.w)
    gfError(__FUNCTION__, "Error: Texture w is 0");
  
  if(!texture.h)
    gfError(__FUNCTION__, "Error: Texture h is 0");
  
  if(!texture.format)
    gfError(__FUNCTION__, "Error: Texture format is 0");
  
  if(!texture.count)  texture.count  = 1;
  if(!texture.mipmap) texture.mipmap = 1;

  glCreateTextures(texture.cubemap ? GL_TEXTURE_CUBE_MAP_ARRAY : GL_TEXTURE_2D_ARRAY, 1, &texture.id);
  glTextureStorage3D(texture.id, texture.mipmap, texture.format, texture.w, texture.h, texture.count * (texture.cubemap ? 6 : 1));
  
  return texture;
}

gpu_texture_t gfTextureCreateFromFile(
  i32 width,
  i32 height,
  i32 mipmap,
  i32 texture_count,
  const char ** texture_paths)
{
  gpu_texture_t texture;
  
  texture.w       = width;
  texture.h       = height;
  texture.count   = texture_count;
  texture.format  = srgb_b8;
  texture.mipmap  = mipmap;
  texture.cubemap = 0;
  
  texture = gfTextureCreateFromStruct(texture);
  
  for(i32 i = 0; i < texture_count; ++i)
  {
    image_t bmp = image(.url = texture_paths[i], .n = 3);
    if(!bmp.w) gfError("Error: File not found", texture_paths[i]);
    gfTextureSetPixels(texture.id, i, 0, 0, texture.w, texture.h, GL_RGB, bmp.pixels);
    image_drop(&bmp);
  }

  glGenerateTextureMipmap(texture.id);

  return texture;
}

gpu_texture_t gfCubemapCreateFromFile(
  i32 width,
  i32 height,
  i32 mipmap,
  i32 texture_count,
  const char ** pos_x_texture_paths,
  const char ** neg_x_texture_paths,
  const char ** pos_y_texture_paths,
  const char ** neg_y_texture_paths,
  const char ** pos_z_texture_paths,
  const char ** neg_z_texture_paths)
{
  gpu_texture_t texture;
  
  texture.w       = width;
  texture.h       = height;
  texture.count   = texture_count;
  texture.format  = srgb_b8;
  texture.mipmap  = mipmap;
  texture.cubemap = 1;
  
  texture = gfTextureCreateFromStruct(texture);
  
  for(i32 i = 0; i < texture_count; ++i)
  {
    image_t pos_x = image(.url = pos_x_texture_paths[i], .n = 3);
    image_t neg_x = image(.url = neg_x_texture_paths[i], .n = 3);
    image_t pos_y = image(.url = pos_y_texture_paths[i], .n = 3);
    image_t neg_y = image(.url = neg_y_texture_paths[i], .n = 3);
    image_t pos_z = image(.url = pos_z_texture_paths[i], .n = 3);
    image_t neg_z = image(.url = neg_z_texture_paths[i], .n = 3);

    if(!pos_x.w) gfError("Error: File not found", pos_x_texture_paths[i]);
    if(!neg_x.w) gfError("Error: File not found", neg_x_texture_paths[i]);
    if(!pos_y.w) gfError("Error: File not found", pos_y_texture_paths[i]);
    if(!neg_y.w) gfError("Error: File not found", neg_y_texture_paths[i]);
    if(!pos_z.w) gfError("Error: File not found", pos_z_texture_paths[i]);
    if(!neg_z.w) gfError("Error: File not found", neg_z_texture_paths[i]);
    
    gfTextureSetPixels(texture.id, i * 6 + 0, 0, 0, texture.w, texture.h, GL_RGB, pos_x.pixels);
    gfTextureSetPixels(texture.id, i * 6 + 1, 0, 0, texture.w, texture.h, GL_RGB, neg_x.pixels);
    gfTextureSetPixels(texture.id, i * 6 + 2, 0, 0, texture.w, texture.h, GL_RGB, pos_y.pixels);
    gfTextureSetPixels(texture.id, i * 6 + 3, 0, 0, texture.w, texture.h, GL_RGB, neg_y.pixels);
    gfTextureSetPixels(texture.id, i * 6 + 4, 0, 0, texture.w, texture.h, GL_RGB, pos_z.pixels);
    gfTextureSetPixels(texture.id, i * 6 + 5, 0, 0, texture.w, texture.h, GL_RGB, neg_z.pixels);
    
    image_drop(&pos_x);
    image_drop(&neg_x);
    image_drop(&pos_y);
    image_drop(&neg_y);
    image_drop(&pos_z);
    image_drop(&neg_z);
  }

  glGenerateTextureMipmap(texture.id);

  return texture;
}

gpu_sampler_t gfSamplerCreateFromStruct(gpu_sampler_t sampler)
{
  if(!sampler.aniso) sampler.aniso = 1;
  if(!sampler.min)   sampler.min   = GL_LINEAR_MIPMAP_LINEAR;
  if(!sampler.mag)   sampler.mag   = GL_LINEAR;
  if(!sampler.wrap)  sampler.wrap  = GL_REPEAT;

  glCreateSamplers(1, &sampler.id);
  
  glSamplerParameteri(sampler.id, GL_TEXTURE_MAX_ANISOTROPY, sampler.aniso);
  glSamplerParameteri(sampler.id, GL_TEXTURE_MIN_FILTER, sampler.min);
  glSamplerParameteri(sampler.id, GL_TEXTURE_MAG_FILTER, sampler.mag);
  glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_S, sampler.wrap);
  glSamplerParameteri(sampler.id, GL_TEXTURE_WRAP_T, sampler.wrap);

  return sampler;
}

void gfTextureSetPixels(
  u32 texture_id,
  i32 texture_layer,
  i32 x,
  i32 y,
  i32 width,
  i32 height,
  u32 format,
  const void * pixels)
{
  glTextureSubImage3D(texture_id, 0, x, y, texture_layer, width, height, 1, format, GL_UNSIGNED_BYTE, pixels);
}

void gfTextureGetPixels(
  u32 texture_id,
  i32 texture_layer,
  i32 x,
  i32 y,
  i32 width,
  i32 height,
  u32 format,
  i32 pixels_bytes,
  void * pixels)
{
  glGetTextureSubImage(texture_id, 0, x, y, texture_layer, width, height, 1, format, GL_UNSIGNED_BYTE, pixels_bytes, pixels);
}

void gfTextureSaveToFile(
  u32 texture_id,
  i32 texture_layer,
  i32 width,
  i32 height,
  const char * pathfile)
{
  const char *ext = strrchr(pathfile, '.');
  if( !ext ) return;

  i32  pixels_bytes = width * height * 3;
  u8 * pixels = (u8 *)malloc((u32)pixels_bytes);
  
  gfTextureGetPixels(texture_id, texture_layer, 0, 0, width, height, GL_RGB, pixels_bytes, pixels);
  
  stbi_flip_vertically_on_write(1);

  /**/ if( !strcmp(ext, ".bmp") ) stbi_write_bmp(pathfile, width, height, 3, pixels);
  else if( !strcmp(ext, ".png") ) stbi_write_png(pathfile, width, height, 3, pixels, width * 3);
  else if( !strcmp(ext, ".tga") ) stbi_write_tga(pathfile, width, height, 3, pixels);
  else if( !strcmp(ext, ".jpg") ) stbi_write_jpg(pathfile, width, height, 3, pixels, 80);
  
  free(pixels);
}

u32 gfFboCreate(
  u32 depth_texture_id,
  i32 depth_texture_layer,
  i32 color_texture_count,
  const u32 * color_texture_ids,
  const i32 * color_texture_layers)
{
  if(color_texture_count < 0 || color_texture_count > 8)
    gfError(__FUNCTION__, "Error: Number of color attachments must be in [0,8]");

  u32 fbo;
  glCreateFramebuffers(1, &fbo);
  
  if(depth_texture_id)
    glNamedFramebufferTextureLayer(fbo, GL_DEPTH_ATTACHMENT, depth_texture_id, 0, depth_texture_layer);
  
  if(color_texture_count)
  {
    u32 attachments[8] = {0};
    
    for(i32 i = 0; i < color_texture_count; ++i)
    {
      attachments[i] = GL_COLOR_ATTACHMENT0 + (u32)i;
      glNamedFramebufferTextureLayer(fbo, GL_COLOR_ATTACHMENT0 + (u32)i, color_texture_ids[i], 0, color_texture_layers ? color_texture_layers[i] : 0);
    }
    
    glNamedFramebufferDrawBuffers(fbo, color_texture_count, attachments);
  }
  
  return fbo;
}

void gfFboBind(u32 fbo_id)
{
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
}

u32 gfProgramCreateFromFile(
  u32 shader_type,
  const char * shader_filepath)
{
  const char * shader_string = file_read(lookup(shader_filepath)).str;
  if( !shader_string ) gfError("Error: File not found", shader_filepath);
  
  uint32_t program = glCreateShaderProgramv(shader_type, 1, (const char **)&shader_string);

  GLint success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
      glGetProgramInfoLog(program, 512, NULL, infoLog);
      // puts(shader_string);
      int line = 0; for each_string(s, shader_string, "\r\n") printf("%03d: %s\n", ++line, s);
      gfError("Vertex Shader Link Error", infoLog);
  }

  return program;
}

u32 gfProgramCreateFromString(
  u32 shader_type,
  const char * shader_string)
{
  return glCreateShaderProgramv(shader_type, 1, (const char **)&shader_string);
}

u32 gfProgramPipelineCreate(
  u32 vert_program,
  u32 frag_program)
{
  u32 ppo;
  glCreateProgramPipelines(1, &ppo);
  
  if(vert_program) glUseProgramStages(ppo, GL_VERTEX_SHADER_BIT, vert_program);
  if(frag_program) glUseProgramStages(ppo, GL_FRAGMENT_SHADER_BIT, frag_program);
  
  char infoLog[512] = {0};
  glValidateProgramPipeline(ppo);
  glGetProgramPipelineInfoLog(ppo, 512, NULL, infoLog);
  if (infoLog[0]) {
      gfError("Program Pipeline Validation Error", infoLog);
  }

  return ppo;
}

void gfClear()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfDraw(
  u32 program_pipeline,
  u32 gpu_cmd_count,
  const gpu_cmd_t * gpu_cmd)
{
  glBindProgramPipeline(program_pipeline);
  
  for(u32 i = 0; i < gpu_cmd_count; ++i)
  {
    gpu_cmd_t cmd = gpu_cmd[i];
    glDrawArraysInstancedBaseInstance(GL_TRIANGLES, (i32)cmd.first, (i32)cmd.count, (i32)cmd.instance_count, cmd.instance_first);
  }
  
  glBindProgramPipeline(0);
}

void gfFire(
  u32 program_pipeline,
  u32 count)
{
  gpu_cmd_t cmd;
  cmd.first = 0;
  cmd.count = count;
  cmd.instance_first = 0;
  cmd.instance_count = 1;
  gfDraw(program_pipeline, 1, &cmd);
}

bool gfCheckExtensions(unsigned glVersion, unsigned extensions_count, const char **extensions) {
    bool pass = true;

    GLint num_extensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    for(int j = 0; j < extensions_count; ++j) {

        int is_ext_supported = 0;
        for (GLint i = 0; i < num_extensions; i++) {
            const char *extension = (const char *)glGetStringi(GL_EXTENSIONS, i);
            if (extension) {
                int core_since = atoi(extensions[j]);
                if( glVersion >= core_since )
                if( strcmp(extension, extensions[j]+2) == 0 ) {
                    is_ext_supported = 1;
                    break;
                }
            }
        }

        if( !is_ext_supported )
            pass = false,
            gfError("Error: Unsupported OpenGL Extension", extensions[j]+2);
    }

    return pass;
}

void gfInit(void)
{
  const char *extensions[] = {
      /*OpenGL 3.0*/ "30GL_ARB_framebuffer_object", "30GL_ARB_framebuffer_sRGB",
      /*OpenGL 3.1*/ "31GL_ARB_texture_buffer_object_rgb32",
      /*OpenGL 3.2*/ "32GL_ARB_seamless_cube_map",
      /*OpenGL 3.3*/ "33GL_ARB_sampler_objects",
      /*OpenGL 4.0*/ "40GL_ARB_texture_cube_map_array", "40GL_ARB_texture_buffer_object_rgb32",
      /*OpenGL 4.1*/ "41GL_ARB_shader_precision", "41GL_ARB_get_program_binary", "41GL_ARB_separate_shader_objects",
      /*OpenGL 4.2*/ "42GL_ARB_base_instance", "42GL_ARB_texture_storage", "42GL_ARB_conservative_depth", "42GL_ARB_shading_language_420pack", "42GL_ARB_shading_language_packing",
      /*OpenGL 4.3*/ "43GL_KHR_debug", "43GL_ARB_explicit_uniform_location",
      /*OpenGL 4.4*/ "44GL_ARB_multi_bind", "44GL_ARB_buffer_storage",
      /*OpenGL 4.5*/ "45GL_ARB_direct_state_access", "45GL_ARB_get_texture_sub_image",
  };
  
  int major; glGetIntegerv(GL_MAJOR_VERSION, &major);
  int minor; glGetIntegerv(GL_MINOR_VERSION, &minor);
  gfCheckExtensions(major * 10 + minor, sizeof(extensions) / sizeof(extensions[0]), extensions);
  
  u32 vao;
  glCreateVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glBlendFunc(0x0302, 0x0303);
  glDepthRange(-1.0, 1.0);
  
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
  glEnable(GL_FRAMEBUFFER_SRGB);
}

void gfError(
  const char * title,
  const char * description)
{
  fprintf(stderr, "glFast: %s: %s\n", title, description);
  alert(title, description);
}

#endif // GLFAST_IMPLEMENTATION
