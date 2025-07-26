BOB: Binary object mesh converter
---------------------------------

This application converts .obj files to a binary form which I call .bob

To compile bob converter on Linux, run:

```bash
cc -o bob bob.c -std=c89
```

To compile bob converter on Windows, run from Developer Command Prompt:

```
cl bob.c
```

To convert .obj to .bob:

```bash
./bob suzanne.obj
```

Bob makes three assumptions:

 1. .obj should be triangulated.
 2. .obj should have 3 attributes: positions, uvs and normals. If your .obj doesn't have uvs or normals, load it in Blender and unwrap the mesh, making sure "Write Normals" flag is enabled on export. 
 3. .obj shouldn't have [negative face indices](https://en.wikipedia.org/wiki/Wavefront_.obj_file#Relative_and_absolute_indices). If your .obj file has them, load it in Blender and re-export as .obj again.

.bob's are easier to load than .obj's. That's all you need to load a .bob file:

```c
// vp - vertex position
// vt - vertex texture uv
// vn - vertex normal
// id - face element id

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef uint32_t u32;
typedef float    f32;

int main()
{
  FILE * fd = fopen("mesh.bob", "rb");

  u32 bytes_vp;
  u32 bytes_vt;
  u32 bytes_vn;
  u32 bytes_id;

  fread(&bytes_vp, sizeof(u32), 1, fd);
  fread(&bytes_vt, sizeof(u32), 1, fd);
  fread(&bytes_vn, sizeof(u32), 1, fd);
  fread(&bytes_id, sizeof(u32), 1, fd);

  f32 * alloc_vp = malloc(bytes_vp);
  f32 * alloc_vt = malloc(bytes_vt);
  f32 * alloc_vn = malloc(bytes_vn);
  u32 * alloc_id = malloc(bytes_id);

  fread(alloc_vp, bytes_vp, 1, fd);
  fread(alloc_vt, bytes_vt, 1, fd);
  fread(alloc_vn, bytes_vn, 1, fd);
  fread(alloc_id, bytes_id, 1, fd);

  fclose(fd);

  free(alloc_vp);
  free(alloc_vt);
  free(alloc_vn);
  free(alloc_id);
  
  return 0;
}
```

Format layout:

.bob:

```
[ u32 vp_bytes, u32 vt_bytes, u32 vn_bytes, u32 id_bytes, f32 px, f32 py, f32 pz, ..., f32 tx, f32 ty, ..., f32 nx, f32 ny, f32 nz, ..., u32 id, u32 id, u32 id, u32 id, u32 id, u32 id, u32 id, u32 id, u32 id, ... ]
```

.ibo:

```
[ f32 px, f32 py, f32 pz, f32 tx, f32 nx, f32 ny, f32 nz, f32 ty, ... ]
```

.h:

```
[ f32 px, f32 py, f32 pz, f32 tx, f32 nx, f32 ny, f32 nz, f32 ty, ... ]
```

For more, see `./bob -h`

**License**: Public domain
