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

#ifndef BOB_IMPLEMENTATION
#define BOB_IMPLEMENTATION
#include "bob.h"
#endif

#ifndef QFPC_IMPLEMENTATION
#define QFPC_IMPLEMENTATION
#include "qfpc.h"
#endif

typedef struct
{
    float sd_x, up_x, fw_x;
    float sd_y, up_y, fw_y;
    float sd_z, up_z, fw_z;
} mat3;

struct
{
    const char *teapot_bob;
    const char *sphere_bob;
    const char *monkey_bob;
    const char *vs_mesh   ;
    const char *fs_mesh   ;
    const char *vs_quad   ;
    const char *fs_quad   ;
    const char *vs_cubemap;
    const char *fs_cubemap;
    const char *texture_1 ;
    const char *texture_2 ;
    const char *texture_3 ;
    const char *cubemap_px;
    const char *cubemap_nx;
    const char *cubemap_py;
    const char *cubemap_ny;
    const char *cubemap_pz;
    const char *cubemap_nz;
} RESRC =
{
    .monkey_bob = "meshes/monkey.bob",
    .sphere_bob = "meshes/teapot.bob", //sphere.bob",
    .teapot_bob = "meshes/teapot.bob",
    .vs_mesh    = "shaders/mesh.vert",
    .fs_mesh    = "shaders/mesh.frag",
    .vs_quad    = "shaders/quad.vert",
    .fs_quad    = "shaders/quad.frag",
    .vs_cubemap = "shaders/cubemap.vert",
    .fs_cubemap = "shaders/cubemap.frag",
    .texture_1  = "textures/texture_1.webp",
    .texture_2  = "textures/texture_2.webp",
    .texture_3  = "textures/texture_3.webp",
    .cubemap_px = "textures/cubemap_right.webp",
    .cubemap_nx = "textures/cubemap_left.webp",
    .cubemap_py = "textures/cubemap_top.webp",
    .cubemap_ny = "textures/cubemap_bottom.webp",
    .cubemap_pz = "textures/cubemap_front.webp",
    .cubemap_nz = "textures/cubemap_back.webp"
};

void Perspective(
    float * proj,
    float aspect,
    float fov_y_rad,
    float n,
    float f)
{
    float d = 1.f / (float)tan(fov_y_rad * 0.5f);
    proj[0] = d / aspect;
    proj[1] = d;
    proj[2] = n / (n - f);
    proj[3] = (f * n) / (n - f);
}

int main() {
    app_create(.title = "glFast(instancingMRT)", .scale = 0.75, .extra = "srgb");

    gfInit();

    const char *bobs[] = {
        RESRC.monkey_bob,
        RESRC.sphere_bob,
        RESRC.teapot_bob,
    };

    bob_t meshes = gfBobCreate(COUNTOF(bobs), bobs);

    const char *bmps[] = {
        RESRC.texture_1,
        RESRC.texture_2,
        RESRC.texture_3,
    };

    gpu_texture_t textures = gfTextureCreateFromFile(512, 512, 4, COUNTOF(bmps), bmps);
    
    const char *cubemap_px[] = { RESRC.cubemap_px };
    const char *cubemap_nx[] = { RESRC.cubemap_nx };
    const char *cubemap_py[] = { RESRC.cubemap_py };
    const char *cubemap_ny[] = { RESRC.cubemap_ny };
    const char *cubemap_pz[] = { RESRC.cubemap_pz };
    const char *cubemap_nz[] = { RESRC.cubemap_nz };
    
    gpu_texture_t cubemaps = gfCubemapCreateFromFile(512, 512, 4, COUNTOF(cubemap_px),
        cubemap_px, cubemap_nx, cubemap_py, cubemap_ny, cubemap_pz, cubemap_nz
    );
    
    unsigned vs_mesh = gfProgramCreateFromFile(GL_VERTEX_SHADER, RESRC.vs_mesh);
    unsigned fs_mesh = gfProgramCreateFromFile(GL_FRAGMENT_SHADER, RESRC.fs_mesh);
    unsigned pp_mesh = gfProgramPipelineCreate(vs_mesh, fs_mesh);
    
    unsigned vs_quad = gfProgramCreateFromFile(GL_VERTEX_SHADER, RESRC.vs_quad);
    unsigned fs_quad = gfProgramCreateFromFile(GL_FRAGMENT_SHADER, RESRC.fs_quad);
    unsigned pp_quad = gfProgramPipelineCreate(vs_quad, fs_quad);
    
    unsigned vs_cubemap = gfProgramCreateFromFile(GL_VERTEX_SHADER, RESRC.vs_cubemap);
    unsigned fs_cubemap = gfProgramCreateFromFile(GL_FRAGMENT_SHADER, RESRC.fs_cubemap);
    unsigned pp_cubemap = gfProgramPipelineCreate(vs_cubemap, fs_cubemap);
    
    gpu_cmd_t cmd[3] = {0};
    
    cmd[0].first = meshes.first.as_u32[0];
    cmd[1].first = meshes.first.as_u32[1];
    cmd[2].first = meshes.first.as_u32[2];
    
    cmd[0].count = meshes.count.as_u32[0];
    cmd[1].count = meshes.count.as_u32[1];
    cmd[2].count = meshes.count.as_u32[2];
    
    cmd[0].instance_first = 0;
    cmd[1].instance_first = 30;
    cmd[2].instance_first = 60;
    
    cmd[0].instance_count = 30;
    cmd[1].instance_count = 30;
    cmd[2].instance_count = 30;
    
    gpu_storage_t ins_first = gfStorageCreate(.format = x_u32, .count = COUNTOF(cmd));
    gpu_storage_t ins_pos = gfStorageCreate(.format = xyzw_f32, .count = 90);
    
    for(unsigned i = 0; i < ins_first.count; ++i) {
        ins_first.as_u32[i] = cmd[i].instance_first;
    }
    
    for(unsigned i = 0, row = 10, space = 3; i < 90; ++i) {
        ins_pos.as_vec4[i].x = i * space - (i / row) * row * space;
        ins_pos.as_vec4[i].y = 0;
        ins_pos.as_vec4[i].z = (i / row) * space;
        ins_pos.as_vec4[i].w = 0;
    }

    gpu_texture_t fbo_depth = gfTextureCreate(.w = app_width(), app_height(), .format = depth_b32);
    gpu_texture_t fbo_color = gfTextureCreate(.w = app_width(), app_height(), .format = srgba_b8);
    
    unsigned fbo_colors[] = {
        [0] = fbo_color.id,
    };
    
    unsigned fbo = gfFboCreate(fbo_depth.id, 0, COUNTOF(fbo_colors), fbo_colors, 0);
    
    gpu_sampler_t s_textures = gfSamplerCreate(); // .aniso = 4);
    gpu_sampler_t s_fbo = gfSamplerCreate(.min = GL_NEAREST, GL_NEAREST);
    
    // Instead of single glBindTextures:
    unsigned state_buffers[8] = {
        [0] = meshes.mesh_id.id,
        [1] = meshes.attr_first.id,
        [2] = meshes.attr_id.id,
        [3] = meshes.pos.id,
        [4] = meshes.uv.id,
        [5] = meshes.normal.id,
        [6] = ins_first.id,
        [7] = ins_pos.id,
    };

    unsigned state_textures[3] = {
        [0] = textures.id,
        [1] = cubemaps.id,
        [2] = fbo_color.id,
    };

    unsigned state_samplers[16] = {
        [8] = s_textures.id,
        [9] = s_textures.id,
        [10] = s_fbo.id,
    };

    for (unsigned i = 0; i < 8; ++i) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, state_buffers[i]);
    }

    glBindTextures(8, 3, state_textures);  // Textures start at unit 8
    glBindSamplers(0, 16, state_samplers);
    
    vec3 cam_pos = {23.518875f, 5.673130f, 26.649000f};
    vec4 cam_rot = {-0.351835f, 0.231701f, 0.090335f, 0.902411f};
    vec4 cam_prj = {0.f};
    mat3 cam_mat = {0.f};

    float aspect = app_width() / (float)app_height();

    Perspective(
        &cam_prj.x,
        aspect,
        85.f * QFPC_TO_RAD,
        0.01f, 1000.f
    );
    
    unsigned t_prev = time_ms();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);

    while(app_swap())
    {
#if 1
        GLint msaa; glGetIntegerv(GL_MULTISAMPLE, &msaa);
        ui_label(va("%5.2f/%5.2f fps, %5.2f ms/f msaa%d", fps(), hz(), spf()*1e3, msaa));

        glEnable(GL_DEPTH_TEST); glDepthRange(-1.0, 1.0);
        glEnable(GL_CULL_FACE);  glCullFace(GL_BACK);
        glEnable(GL_BLEND);      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#endif

        unsigned t_curr = time_ms();
        double dt = ((t_curr - t_prev) * 60.0) / 1000.0;

#if 1
        static int omx, omy;
        int mx = mouse('x');
        int my = mouse('y');
        int mouse_x_rel = mx - omx; omx = mx;
        int mouse_y_rel = my - omy; omy = my;
        ONCE mouse_x_rel = mouse_y_rel = 0;
#else
        ( mouse('l') ? RGFW_window_holdMouse : RGFW_window_unholdMouse)(app_handle());
        int mouse_x_rel = mouse('x');
        int mouse_y_rel = mouse('y');
#endif

        if( !mouse('l') ) mouse_cursor(1);
        else
        mouse_cursor(0),
        quatFirstPersonCamera(
            &cam_pos.x,
            &cam_rot.x,
            &cam_mat.sd_x,
            0.10f,
            0.05f * (float)dt,
            mouse_x_rel,
            mouse_y_rel,
            key('W'),
            key('A'),
            key('S'),
            key('D'),
            key('E'),
            key('Q')
        );
        
        static int show_pass = 0;
        if(key('1')) show_pass = 0;
        if(key('2')) show_pass = 1;
        if(key('3')) show_pass = 2;
        if(key('4')) show_pass = 3;
        if(key('5')) show_pass = 4;
        
        glProgramUniform3fv(vs_mesh, 0, 1, &cam_pos.x);
        glProgramUniform4fv(vs_mesh, 1, 1, &cam_rot.x);
        glProgramUniform4fv(vs_mesh, 2, 1, &cam_prj.x);
        glProgramUniform3fv(fs_mesh, 0, 1, &cam_pos.x);
        glProgramUniform1iv(fs_mesh, 1, 1, &show_pass);
        glProgramUniform4fv(vs_cubemap, 0, 1, &cam_rot.x);
        glProgramUniform4fv(vs_cubemap, 1, 1, &cam_prj.x);
        
        for(unsigned i = 0; i < 90; ++i)
            ins_pos.as_vec4[i].y = (float)sin((t_curr * 0.0015f) + (i * 0.5f)) * 0.3f;
        
        gfFboBind(fbo);
            gfClear();
            gfDraw(pp_mesh, COUNTOF(cmd), cmd);
        
            // Render skybox
            if(!show_pass)
            {
                // Disable writing to the depth buffer during skybox rendering. Since the skybox is meant to be the farthest thing in the scene (at the far plane, depth = 1.0), we don’t want it to overwrite depth values in the buffer. This preserves the depth information from the scene, which might be needed for post-processing or additional rendering passes.
                glDepthMask(GL_FALSE);
                // We set the depth to 1.0 (the far plane) and use GL_LEQUAL to allow drawing where the depth is less than or equal to 1.0. Since the scene’s objects have depth values < 1.0, the skybox only draws in areas where the depth buffer is still 1.0 (unoccupied pixels).
                // see also: Depth Value in the Vertex Shader `gl_Position.z = gl_Position.w;` This line ensures the skybox’s depth is set to 1.0 in normalized device coordinates (NDC)
                glDepthFunc(GL_LEQUAL);


                    gfFire(pp_cubemap, 36);


                // Restore depth state
                glDepthMask(GL_TRUE);
                glDepthFunc(GL_LESS);
            }
        
        gfFboBind(0);

        gfFire(pp_quad, 6);

        t_prev = t_curr;
    }

//  gfTextureSaveToFile(fbo_color.id, 0, fbo_color.w, fbo_color.h, "debug.png");

    return 0;
}
