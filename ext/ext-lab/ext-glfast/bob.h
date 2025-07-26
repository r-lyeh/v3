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

#ifndef BOB_H_
#define BOB_H_

// BOB [BINARY OBJECT] MESH LOADER /////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// TYPES ///////////////////////////////////////////////////////////////////////

typedef struct
{
  // For gpu_cmd_t
  gpu_storage_t first;
  gpu_storage_t count;
  // For attributes
  gpu_storage_t mesh_id;
  gpu_storage_t attr_first;
  gpu_storage_t attr_id;
  gpu_storage_t pos;
  gpu_storage_t uv;
  gpu_storage_t normal;
} bob_t;

// PROTOTYPES //////////////////////////////////////////////////////////////////

void gfBobReadHeader(const char * path, unsigned * bytes_vp, unsigned * bytes_vt, unsigned * bytes_vn, unsigned * bytes_id);
void gfBobRead(const char * path, unsigned bytes_vp, void ** alloc_vp, unsigned bytes_vt, void ** alloc_vt, unsigned bytes_vn, void ** alloc_vn, unsigned bytes_id, void ** alloc_id);
bob_t gfBobCreate(unsigned bob_count, const char ** bob_paths);

#ifdef __cplusplus
}
#endif
#endif // BOB_H_

// IMPLEMENTATION //////////////////////////////////////////////////////////////

#ifdef BOB_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gfBobReadHeader(
  const char * path,
  unsigned * bytes_vp,
  unsigned * bytes_vt,
  unsigned * bytes_vn,
  unsigned * bytes_id)
{
  FILE * fd = fopen(path, "rb");
  if(fd == NULL)
    gfError("Error: File Not Found", path);
  
  fread(bytes_vp, sizeof(unsigned), 1, fd);
  fread(bytes_vt, sizeof(unsigned), 1, fd);
  fread(bytes_vn, sizeof(unsigned), 1, fd);
  fread(bytes_id, sizeof(unsigned), 1, fd);

  fclose(fd);
}

void gfBobRead(
  const char * path,
  unsigned bytes_vp, void ** alloc_vp,
  unsigned bytes_vt, void ** alloc_vt,
  unsigned bytes_vn, void ** alloc_vn,
  unsigned bytes_id, void ** alloc_id)
{
  FILE * fd = fopen(path, "rb");
  if(fd == NULL)
    gfError("Error: File Not Found", path);
  
  fseek(fd, 4 * sizeof(unsigned), SEEK_CUR);
  fread(*alloc_vp, bytes_vp, 1, fd);
  fread(*alloc_vt, bytes_vt, 1, fd);
  fread(*alloc_vn, bytes_vn, 1, fd);
  fread(*alloc_id, bytes_id, 1, fd);

  fclose(fd);

  lputs(va("`%s` decoded with bob .vp=%u .vn=%u .vt=%u (%u tris)", path, bytes_vp/12, bytes_vn/12, bytes_vt/8, bytes_id/12));
}

bob_t gfBobCreate(
  unsigned bob_count,
  const char ** bob_paths)
{
  bob_t bobs = {0};

  bobs.first.format = x_u32;
  bobs.count.format = x_u32;

  bobs.first.count = bob_count;
  bobs.count.count = bob_count;

  bobs.first = gfStorageCreateFromStruct(bobs.first);
  bobs.count = gfStorageCreateFromStruct(bobs.count);
  
  unsigned mesh_id_format = x_u32;
#if 0 // removed: always u32 to simplify SSBO access as int[] or uint[
  if(bob_count <= 65535+1) mesh_id_format = x_u16;
  if(bob_count <= 255+1)   mesh_id_format = x_u8;
#endif
  
  bobs.mesh_id   .format = mesh_id_format;
  bobs.attr_first.format = xyzw_u32; // Padded to ivec4
  bobs.attr_id   .format = xyzw_u32; // Padded to ivec4
  bobs.pos       .format = xyzw_f32; // Padded to vec4
  bobs.uv        .format = xy_f32;
  bobs.normal    .format = xyzw_f32; // Padded to vec4
  
  unsigned * bob_bytes_vp = (unsigned *)malloc(bob_count * sizeof(unsigned));
  unsigned * bob_bytes_vt = (unsigned *)malloc(bob_count * sizeof(unsigned));
  unsigned * bob_bytes_vn = (unsigned *)malloc(bob_count * sizeof(unsigned));
  unsigned * bob_bytes_id = (unsigned *)malloc(bob_count * sizeof(unsigned));
  
  for(unsigned i = 0; i < bob_count; ++i)
  {
    gfBobReadHeader(
       bob_paths[i],
      &bob_bytes_vp[i],
      &bob_bytes_vt[i],
      &bob_bytes_vn[i],
      &bob_bytes_id[i]
    );
    bobs.mesh_id   .bytes += bob_bytes_id[i] / 3;
    bobs.attr_first.bytes += sizeof(unsigned) * 4;
    bobs.attr_id   .bytes += (bob_bytes_id[i] / 12) * 16;
    bobs.pos       .bytes += (bob_bytes_vp[i] / 12) * 16;
    bobs.uv        .bytes += bob_bytes_vt[i];
    bobs.normal    .bytes += (bob_bytes_vn[i] / 12) * 16;
  }
  
  for(unsigned i = 0; i < bob_count; ++i)
  {
    bobs.count.as_uint32[i] = bob_bytes_id[i] / sizeof(unsigned) / 3;
  }
  
  for(unsigned i = 0, total_idx_count = 0; i < bob_count; ++i)
  {
    bobs.first.as_uint32[i] = total_idx_count;
    total_idx_count += bobs.count.as_uint32[i];
  }
  
  bobs.mesh_id    = gfStorageCreateFromStruct(bobs.mesh_id);
  bobs.attr_first = gfStorageCreateFromStruct(bobs.attr_first);
  bobs.attr_id    = gfStorageCreateFromStruct(bobs.attr_id);
  bobs.pos        = gfStorageCreateFromStruct(bobs.pos);
  bobs.uv         = gfStorageCreateFromStruct(bobs.uv);
  bobs.normal     = gfStorageCreateFromStruct(bobs.normal);
  
  for(unsigned i = 0,
          j = 0,
   vp_first = 0,
   vt_first = 0,
   vn_first = 0; i < bob_count; ++i)
  {
    unsigned num_verts_p = bob_bytes_vp[i] / 12;
    unsigned num_verts_t = bob_bytes_vt[i] / 8;
    unsigned num_verts_n = bob_bytes_vn[i] / 12;
    
    bobs.attr_first.as_uint32[j++] = vp_first;
    bobs.attr_first.as_uint32[j++] = vt_first;
    bobs.attr_first.as_uint32[j++] = vn_first;
    bobs.attr_first.as_uint32[j++] = 0;
    
    vp_first += num_verts_p;
    vt_first += num_verts_t;
    vn_first += num_verts_n;
  }
  
  if(mesh_id_format == x_u8)
  {
    for(u8 i = 0; i < bob_count; ++i)
    {
      const unsigned first = bobs.first.as_uint32[i];
      const unsigned count = bobs.count.as_uint32[i];
      memset(bobs.mesh_id.ptr + first, i, count);
    }
  }
  else
  if(mesh_id_format == x_u16)
  {
    for(u16 i = 0; i < bob_count; ++i)
    {
      const unsigned first = bobs.first.as_uint32[i];
      const unsigned count = bobs.count.as_uint32[i] + first;
      for(unsigned j = first; j < count; ++j)
        bobs.mesh_id.as_uint16[j] = i;
    }
  }
  else
  {
    for(unsigned i = 0; i < bob_count; ++i)
    {
      const unsigned first = bobs.first.as_uint32[i];
      const unsigned count = bobs.count.as_uint32[i] + first;
      for(unsigned j = first; j < count; ++j)
        bobs.mesh_id.as_uint32[j] = i;
    }
  }
  
  for(unsigned i = 0,
   vp_offset = 0,
   vt_offset = 0,
   vn_offset = 0,
   id_offset = 0; i < bob_count; ++i)
  {
    unsigned num_verts_p = bob_bytes_vp[i] / 12;
    unsigned num_verts_t = bob_bytes_vt[i] / 8;
    unsigned num_verts_n = bob_bytes_vn[i] / 12;
    unsigned num_attr = bob_bytes_id[i] / 12;
    
    float *temp_vp = (float *)malloc(bob_bytes_vp[i]);
    float *temp_vt = (float *)malloc(bob_bytes_vt[i]);
    float *temp_vn = (float *)malloc(bob_bytes_vn[i]);
    uint32_t *temp_id = (uint32_t *)malloc(bob_bytes_id[i]);
    
    gfBobRead(
      bob_paths[i],
      bob_bytes_vp[i], (void **)&temp_vp,
      bob_bytes_vt[i], (void **)&temp_vt,
      bob_bytes_vn[i], (void **)&temp_vn,
      bob_bytes_id[i], (void **)&temp_id
    );
    
    for (unsigned k = 0; k < num_verts_p; ++k) {
        bobs.pos.as_vec4[vp_offset + k].x = temp_vp[3*k];
        bobs.pos.as_vec4[vp_offset + k].y = temp_vp[3*k + 1];
        bobs.pos.as_vec4[vp_offset + k].z = temp_vp[3*k + 2];
        bobs.pos.as_vec4[vp_offset + k].w = 0.0f;
    }
    
    for (unsigned k = 0; k < num_verts_t; ++k) {
        bobs.uv.as_vec2[vt_offset + k].x = temp_vt[2*k + 0];
        bobs.uv.as_vec2[vt_offset + k].y = temp_vt[2*k + 1];
    }
    
    for (unsigned k = 0; k < num_verts_n; ++k) {
        bobs.normal.as_vec4[vn_offset + k].x = temp_vn[3*k];
        bobs.normal.as_vec4[vn_offset + k].y = temp_vn[3*k + 1];
        bobs.normal.as_vec4[vn_offset + k].z = temp_vn[3*k + 2];
        bobs.normal.as_vec4[vn_offset + k].w = 0.0f;
    }
    
    for (unsigned k = 0; k < num_attr; ++k) {
        bobs.attr_id.as_uint32[4*(id_offset + k)] = temp_id[3*k];
        bobs.attr_id.as_uint32[4*(id_offset + k) + 1] = temp_id[3*k + 1];
        bobs.attr_id.as_uint32[4*(id_offset + k) + 2] = temp_id[3*k + 2];
        bobs.attr_id.as_uint32[4*(id_offset + k) + 3] = 0;
    }
    
    free(temp_vp);
    free(temp_vt);
    free(temp_vn);
    free(temp_id);
    
    vp_offset += num_verts_p;
    vt_offset += num_verts_t;
    vn_offset += num_verts_n;
    id_offset += num_attr;
  }
  
  free(bob_bytes_vp);
  free(bob_bytes_vt);
  free(bob_bytes_vn);
  free(bob_bytes_id);
  
  return bobs;
}

#endif // BOB_IMPLEMENTATION
