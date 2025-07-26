/*//////////////////////////////////////////////////////////////////////////////
// BOB [BINARY OBJECT] MESH CONVERTER  /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Compiling: cc -o bob bob.c -std=c89 /////////////////////////////////////////
// Executing: ./bob suzanne.obj        /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* clang-format off */
typedef uint32_t u32;
typedef float f32;
#define streq(x,y) (strcmp(x, y) == 0)
#define bytesof(x) (ptrdiff_t)(sizeof(x))
/* clang-format on */

int main(int ArgCount, char ** Args)
{
  /* clang-format off */
  bool flag_print_bob_info = false;
  bool flag_generate_obj   = false;
  bool flag_generate_ibo   = false;
  bool flag_generate_cfile = false;

  if(ArgCount < 2 ||
     streq(Args[1],  "-h") || streq(Args[1],  "-help") ||
     streq(Args[1], "--h") || streq(Args[1], "--help"))
  {
    printf(
        "Usage: bob [options] file...                                    \n"
        "Options:                                                        \n"
        "  -i       Print information for .bob files                     \n"
        "  -obj     Convert .bobs back to .obj files                     \n"
        "  -ibo     Generate interleaved binary .obj                     \n"
        "  -c       Generate interleaved header .obj for C/C++ languages \n");
    return 0;
  }
  else
  {
    if(streq(Args[1], "-i")) flag_print_bob_info = true;
    else
    if(streq(Args[1], "-obj")) flag_generate_obj = true;
    else
    if(streq(Args[1], "-ibo")) flag_generate_ibo = true;
    else
    if(streq(Args[1], "-c")) flag_generate_cfile = true;
  }
  /* clang-format on */

  if (flag_generate_obj)
  {
    ptrdiff_t i = 2;
    for (; i < ArgCount; ++i)
    {
      FILE * fd;

      fd = fopen(Args[i], "rb");

      if (fd == NULL)
      {
        fprintf(stderr, "Can't open passed .bob file: %s\n", Args[i]);
        continue;
      }

      ptrdiff_t bytes_vp;
      ptrdiff_t bytes_vt;
      ptrdiff_t bytes_vn;
      ptrdiff_t bytes_id;

      fread(&bytes_vp, sizeof(u32), 1, fd);
      fread(&bytes_vt, sizeof(u32), 1, fd);
      fread(&bytes_vn, sizeof(u32), 1, fd);
      fread(&bytes_id, sizeof(u32), 1, fd);

      f32 * alloc_vp = (f32 *)malloc((size_t)bytes_vp);
      f32 * alloc_vt = (f32 *)malloc((size_t)bytes_vt);
      f32 * alloc_vn = (f32 *)malloc((size_t)bytes_vn);
      u32 * alloc_id = (u32 *)malloc((size_t)bytes_id);

      fread(alloc_vp, (size_t)bytes_vp, 1, fd);
      fread(alloc_vt, (size_t)bytes_vt, 1, fd);
      fread(alloc_vn, (size_t)bytes_vn, 1, fd);
      fread(alloc_id, (size_t)bytes_id, 1, fd);

      fclose(fd);

      memcpy(&Args[i][strlen(Args[i]) - 3], "obj", 3);

      fd = fopen(Args[i], "wb");

      /* clang-format off */
      {
        ptrdiff_t j = 0;
        ptrdiff_t end = bytes_vp / 3 / bytesof(f32);
        for (; j < end; ++j)
          fprintf(
              fd, "v %f %f %f\n",
              alloc_vp[j * 3 + 0],
              alloc_vp[j * 3 + 1],
              alloc_vp[j * 3 + 2]);
      }
      {
        ptrdiff_t j = 0;
        ptrdiff_t end = bytes_vt / 2 / bytesof(f32);
        for (; j < end; ++j)
          fprintf(
              fd, "vt %f %f\n",
              alloc_vt[j * 2 + 0],
              alloc_vt[j * 2 + 1]);
      }
      {
        ptrdiff_t j = 0;
        ptrdiff_t end = bytes_vn / 3 / bytesof(f32);
        for (; j < end; ++j)
          fprintf(
              fd, "vn %f %f %f\n",
              alloc_vn[j * 3 + 0],
              alloc_vn[j * 3 + 1],
              alloc_vn[j * 3 + 2]);
      }
      {
        ptrdiff_t j = 0;
        ptrdiff_t end = bytes_id / 9 / bytesof(u32);
        for (; j < end; ++j)
          fprintf(
              fd, "f %u/%u/%u %u/%u/%u %u/%u/%u\n",
              alloc_id[j * 9 + 0] + 1,
              alloc_id[j * 9 + 1] + 1,
              alloc_id[j * 9 + 2] + 1,
              alloc_id[j * 9 + 3] + 1,
              alloc_id[j * 9 + 4] + 1,
              alloc_id[j * 9 + 5] + 1,
              alloc_id[j * 9 + 6] + 1,
              alloc_id[j * 9 + 7] + 1,
              alloc_id[j * 9 + 8] + 1);
      }
      /* clang-format on */

      fclose(fd);

      free(alloc_vp);
      free(alloc_vt);
      free(alloc_vn);
      free(alloc_id);
    }
    return 0;
  }

  if (flag_print_bob_info)
  {
    ptrdiff_t total_count_vp = 0;
    ptrdiff_t total_count_vt = 0;
    ptrdiff_t total_count_vn = 0;
    ptrdiff_t total_count_id = 0;

    ptrdiff_t i = 2;
    for (; i < ArgCount; ++i)
    {
      FILE * fd = fopen(Args[i], "rb");

      if (fd == NULL)
      {
        fprintf(stderr, "Can't open passed .bob file: %s\n", Args[i]);
        continue;
      }

      ptrdiff_t bytes_vp;
      ptrdiff_t bytes_vt;
      ptrdiff_t bytes_vn;
      ptrdiff_t bytes_id;

      fread(&bytes_vp, sizeof(u32), 1, fd);
      fread(&bytes_vt, sizeof(u32), 1, fd);
      fread(&bytes_vn, sizeof(u32), 1, fd);
      fread(&bytes_id, sizeof(u32), 1, fd);

      f32 * alloc_vp = (f32 *)malloc((size_t)bytes_vp);
      f32 * alloc_vt = (f32 *)malloc((size_t)bytes_vt);
      f32 * alloc_vn = (f32 *)malloc((size_t)bytes_vn);
      u32 * alloc_id = (u32 *)malloc((size_t)bytes_id);

      fread(alloc_vp, (size_t)bytes_vp, 1, fd);
      fread(alloc_vt, (size_t)bytes_vt, 1, fd);
      fread(alloc_vn, (size_t)bytes_vn, 1, fd);
      fread(alloc_id, (size_t)bytes_id, 1, fd);

      fclose(fd);

      ptrdiff_t count_vp = bytes_vp / 3 / bytesof(f32);
      ptrdiff_t count_vt = bytes_vt / 2 / bytesof(f32);
      ptrdiff_t count_vn = bytes_vn / 3 / bytesof(f32);
      ptrdiff_t count_id = bytes_id / 9 / bytesof(u32);

      printf("For \"%s\":\n\n", Args[i]);
      printf("Vertex count: %ld\n", count_vp);
      printf("UV count: %ld\n", count_vt);
      printf("Normal count: %ld\n", count_vn);
      printf("Triangle count: %ld\n\n", count_id);

      total_count_vp += count_vp;
      total_count_vt += count_vt;
      total_count_vn += count_vn;
      total_count_id += count_id;

      free(alloc_vp);
      free(alloc_vt);
      free(alloc_vn);
      free(alloc_id);
    }

    printf("Total:\n\n");
    printf("Vertex count: %ld\n", total_count_vp);
    printf("UV count: %ld\n", total_count_vt);
    printf("Normal count: %ld\n", total_count_vn);
    printf("Triangle count: %ld\n", total_count_id);

    return 0;
  }

  ptrdiff_t i = 1 + (flag_generate_ibo || flag_generate_cfile);
  for (; i < ArgCount; ++i)
  {
    ptrdiff_t bytes_str;
    char * alloc_str;
    {
      FILE * fd = fopen(Args[i], "rb");

      if (fd == NULL)
      {
        fprintf(stderr, "Can't open passed .obj file: %s\n", Args[i]);
        continue;
      }

      if (!streq(&Args[i][strlen(Args[i]) - 3], "obj"))
      {
        fprintf(stderr, "Passed file is not .obj: %s\n", Args[i]);
        continue;
      }

      fseek(fd, 0, SEEK_END);
      bytes_str = ftell(fd);
      fseek(fd, 0, SEEK_SET);

      alloc_str = (char *)malloc((size_t)bytes_str + 1);
      alloc_str[bytes_str] = 0;
      fread(alloc_str, (size_t)bytes_str, 1, fd);

      fclose(fd);
    }

    f32 * alloc_vp = (f32 *)malloc(0);
    f32 * alloc_vt = (f32 *)malloc(0);
    f32 * alloc_vn = (f32 *)malloc(0);
    u32 * alloc_id = (u32 *)malloc(0);

    ptrdiff_t offset_alloc_vp = 0;
    ptrdiff_t offset_alloc_vt = 0;
    ptrdiff_t offset_alloc_vn = 0;
    ptrdiff_t offset_alloc_id = 0;

    const char * delims = " \n/";
    const char * str = strtok(alloc_str, delims);
    for (; str != NULL; str = strtok(NULL, delims))
    {
#define ATTRIB(X) streq(str, X)

#define APPEND(X, SIZE, TYPE)                                                  \
  {                                                                            \
    X = (TYPE *)realloc(X, (size_t)(offset_##X + SIZE) * sizeof(TYPE));        \
  }

#define NEXT(X, SIZE)                                                          \
  {                                                                            \
    offset_##X += SIZE;                                                        \
  }

#define READ_FLOAT(X, POS)                                                     \
  {                                                                            \
    str = strtok(NULL, delims);                                                \
    X[offset_##X + POS] = (f32)strtod(str, NULL);                              \
  }

#define READ_INDEX(X, POS)                                                     \
  {                                                                            \
    str = strtok(NULL, delims);                                                \
    X[offset_##X + POS] = (u32)strtoul(str, NULL, 0) - 1;                      \
  }

      if (ATTRIB("v"))
      {
        APPEND(alloc_vp, 3, f32)

        READ_FLOAT(alloc_vp, 0)
        READ_FLOAT(alloc_vp, 1)
        READ_FLOAT(alloc_vp, 2)

        NEXT(alloc_vp, 3);

        continue;
      }

      if (ATTRIB("vt"))
      {
        APPEND(alloc_vt, 2, f32)

        READ_FLOAT(alloc_vt, 0)
        READ_FLOAT(alloc_vt, 1)

        NEXT(alloc_vt, 2);

        continue;
      }

      if (ATTRIB("vn"))
      {
        APPEND(alloc_vn, 3, f32)

        READ_FLOAT(alloc_vn, 0)
        READ_FLOAT(alloc_vn, 1)
        READ_FLOAT(alloc_vn, 2)

        NEXT(alloc_vn, 3);

        continue;
      }

      if (ATTRIB("f"))
      {
        APPEND(alloc_id, 9, u32)

        READ_INDEX(alloc_id, 0)
        READ_INDEX(alloc_id, 1)
        READ_INDEX(alloc_id, 2)

        READ_INDEX(alloc_id, 3)
        READ_INDEX(alloc_id, 4)
        READ_INDEX(alloc_id, 5)

        READ_INDEX(alloc_id, 6)
        READ_INDEX(alloc_id, 7)
        READ_INDEX(alloc_id, 8)

        NEXT(alloc_id, 9);

        continue;
      }
    }

    free(alloc_str);

    ptrdiff_t bytes_vp = offset_alloc_vp * bytesof(f32);
    ptrdiff_t bytes_vt = offset_alloc_vt * bytesof(f32);
    ptrdiff_t bytes_vn = offset_alloc_vn * bytesof(f32);
    ptrdiff_t bytes_id = offset_alloc_id * bytesof(u32);

    if (flag_generate_ibo)
    {
      memcpy(&Args[i][strlen(Args[i]) - 3], "ibo", 3);

      FILE * fd = fopen(Args[i], "wb");

      {
        ptrdiff_t j = 0;
        ptrdiff_t end = offset_alloc_id;
        for (; j < end; j += 3)
        {
          f32 vp_x = alloc_vp[alloc_id[j + 0] * 3 + 0];
          f32 vp_y = alloc_vp[alloc_id[j + 0] * 3 + 1];
          f32 vp_z = alloc_vp[alloc_id[j + 0] * 3 + 2];

          f32 vt_x = alloc_vt[alloc_id[j + 1] * 2 + 0];

          f32 vn_x = alloc_vn[alloc_id[j + 2] * 3 + 0];
          f32 vn_y = alloc_vn[alloc_id[j + 2] * 3 + 1];
          f32 vn_z = alloc_vn[alloc_id[j + 2] * 3 + 2];

          f32 vt_y = alloc_vt[alloc_id[j + 1] * 2 + 1];

          fwrite(&vp_x, sizeof(f32), 1, fd);
          fwrite(&vp_y, sizeof(f32), 1, fd);
          fwrite(&vp_z, sizeof(f32), 1, fd);

          fwrite(&vt_x, sizeof(f32), 1, fd);

          fwrite(&vn_x, sizeof(f32), 1, fd);
          fwrite(&vn_y, sizeof(f32), 1, fd);
          fwrite(&vn_z, sizeof(f32), 1, fd);

          fwrite(&vt_y, sizeof(f32), 1, fd);
        }
      }

      fclose(fd);
    }
    else if (flag_generate_cfile)
    {
      memcpy(&Args[i][strlen(Args[i]) - 3], "h\0", 2);

      FILE * fd = fopen(Args[i], "wb");

      fprintf(fd, "float obj[] = {\n");

      {
        ptrdiff_t j = 0;
        ptrdiff_t end = offset_alloc_id;
        for (; j < end; j += 3)
        {
          f32 vp_x = alloc_vp[alloc_id[j + 0] * 3 + 0];
          f32 vp_y = alloc_vp[alloc_id[j + 0] * 3 + 1];
          f32 vp_z = alloc_vp[alloc_id[j + 0] * 3 + 2];

          f32 vt_x = alloc_vt[alloc_id[j + 1] * 2 + 0];

          f32 vn_x = alloc_vn[alloc_id[j + 2] * 3 + 0];
          f32 vn_y = alloc_vn[alloc_id[j + 2] * 3 + 1];
          f32 vn_z = alloc_vn[alloc_id[j + 2] * 3 + 2];

          f32 vt_y = alloc_vt[alloc_id[j + 1] * 2 + 1];

          char fmt[] = "  %+ff, %+ff, %+ff, %+ff, %+ff, %+ff, %+ff, %+ff,\n";

          if (j == end - 3)
            fmt[strlen(fmt) - 2] = ' ';

          fprintf(fd, fmt, vp_x, vp_y, vp_z, vt_x, vn_x, vn_y, vn_z, vt_y);
        }
      }

      fprintf(fd, "};\n");

      fclose(fd);
    }
    else
    {
      memcpy(&Args[i][strlen(Args[i]) - 3], "bob", 3);

      FILE * fd = fopen(Args[i], "wb");

      fwrite(&bytes_vp, sizeof(u32), 1, fd);
      fwrite(&bytes_vt, sizeof(u32), 1, fd);
      fwrite(&bytes_vn, sizeof(u32), 1, fd);
      fwrite(&bytes_id, sizeof(u32), 1, fd);

      fwrite(alloc_vp, (size_t)bytes_vp, 1, fd);
      fwrite(alloc_vt, (size_t)bytes_vt, 1, fd);
      fwrite(alloc_vn, (size_t)bytes_vn, 1, fd);
      fwrite(alloc_id, (size_t)bytes_id, 1, fd);

      fclose(fd);
    }

    free(alloc_vp);
    free(alloc_vt);
    free(alloc_vn);
    free(alloc_id);
  }

  return 0;
}
