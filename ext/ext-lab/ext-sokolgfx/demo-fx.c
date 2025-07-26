#define V3_IMPLEMENTATION
#include "v3.h"

#ifdef image

#include "art/shaders.h"

#define state    state_FX
#define vertex_t vertex_t_FX

#define MAX_PARTICLES (512 * 1024)
#define NUM_PARTICLES_EMITTED_PER_FRAME (10)

#define PI_F 3.1415927f

// Vertex attributes
typedef struct
{
    float pos[2];
    float uv[2];
} vertex_t;

// Particle attributes
typedef struct
{
    float pos[2];
    float color[4];
} particle_t;

// Particle velocity
typedef struct
{
    float x, y;
} vel_t;

// Simulation staTe
static struct
{
    particle_t particles[MAX_PARTICLES];
    int particle_count;
    vel_t vel[MAX_PARTICLES];
} state;

// Generate a random number between min and max
static float random_float(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}

pg_texture_t* load_texture(void *ctx, const char* file)
{
    image_t img = image(.url = file, .ch = 4, .flip = 1);

    assert(img.bitmap && img.c == 4);

    size_t size = img.w * img.h * img.c;
    pg_texture_t* tex = pg_create_texture(ctx, img.w, img.h, img.bitmap, size, NULL);

    image_drop(&img);

    return tex;
}

int main()
{
    memset(&state, 0, sizeof(state));

    assert(pg_backend() == PG_BACKEND_GL);

    app_create(.title = "Particle rendering demo", "0.50");

    int pixel_w = app_width(), pixel_h = app_height();
    int win_w = pixel_w, win_h = pixel_h;

    pg_init();

    // Initialize context
    pg_ctx_t* ctx = pg_create_context(pixel_w, pixel_h, NULL);
    pg_shader_t* shader = pg_create_shader(ctx, particle);

    // Create particle texture

    pg_texture_t* tex = load_texture(ctx, "circle.webp");
    int w = tex->width, h = tex->height;

    // Specify vertices

    vertex_t vertices[6] =
    {
        { { 0, 0, }, { 0, 1 } },
        { { 0, h, }, { 0, 0 } },
        { { w, 0, }, { 1, 1 } },

        { { 0, h, }, { 0, 0 } },
        { { w, h, }, { 1, 0 } },
        { { w, 0, }, { 1, 1 } }
    };

    pg_pipeline_t* pipeline = pg_create_pipeline(ctx, shader, &(pg_pipeline_opts_t)
    {
        .layout =
        {
            .bufs =
            {
                [1] = { .instanced = true } // Buffer in slot 1 is instanced
            },
            .attrs =
            {
                // Attributes for the vertex buffer
                [ATTR_particle_a_pos] = { .format = PG_VERTEX_FORMAT_FLOAT2,
                                          .offset = offsetof(vertex_t, pos) },

                [ATTR_particle_a_uv]  = { .format = PG_VERTEX_FORMAT_FLOAT2,
                                          .offset = offsetof(vertex_t, uv) },

                // Attributes for the instanced buffer
                [ATTR_particle_a_inst_pos]  = { .format = PG_VERTEX_FORMAT_FLOAT2,
                                                .offset = offsetof(particle_t, pos),
                                                .buffer_index = 1 },

                [ATTR_particle_a_inst_color] = { .format = PG_VERTEX_FORMAT_FLOAT4,
                                                 .offset = offsetof(particle_t, color),
                                                 .buffer_index = 1 },
            },
        },
        .blend_enabled = true,
        .blend =
        {
            .color_src = PG_SRC_ALPHA,
            .color_dst = PG_ONE_MINUS_SRC_ALPHA
        }
    });

    // Sets the vertex uniform block mvp to project world to NDC coordinates
    vs_params_t block =
    {
        .u_mvp =
        {
            2.0f / win_w, 0.0f,         0.0f,       0.0f,
            0.0f,        -2.0/ win_h,   0.0f,       0.0f,
            0.0f,         0.0f,         0.0f,       0.0f,
           -1.0f,         1.0f,         0.0f,       1.0f,
        }
    };

    // Initializes and sets the uniform
    pg_set_uniform_block(shader, "vs_params", &block);

    // Create the vertex buffer
    pg_buffer_t* vertex_buffer = pg_create_vertex_buffer(ctx, PG_USAGE_STATIC,
                                                         vertices, 6, 6,
                                                         sizeof(vertex_t));

    // Create the instance buffer
    pg_buffer_t* instance_buffer = pg_create_vertex_buffer(ctx, PG_USAGE_STREAM,
                                                           NULL, 0, MAX_PARTICLES,
                                                           sizeof(particle_t));

    // Create a default sampler
    pg_sampler_t* sampler = pg_create_sampler(ctx, NULL);


    // The main loop
    double delta = 0.0;
    uint64_t now, last = time_ns();

    while (app_swap() && !key(0))
    {
        ui_label(va("Particles: %d", state.particle_count));

        sg_reset_state_cache();

        // Calculate delta
        now = time_ns();
        delta = (now - last) / 1e9;
        last = now;

        // New particles
        for (int i = 0; i < NUM_PARTICLES_EMITTED_PER_FRAME; i++)
        {
            if (state.particle_count >= MAX_PARTICLES)
                break;

            // Calculate a random angle for the particles trajectory
            float angle = random_float(PI_F / 4.f, 3.f * PI_F / 4.f);

            // Set new particle to the center of the screen
            particle_t particle =
            {
                .pos   = { win_w / 2.f, win_h / 2.f },
                .color = { 0.f, 0.f, 0.f, 1.f },
            };

            // Vary the color of the particles
            particle.color[0] =
            particle.color[1] =
            particle.color[2] = random_float(0.75, 1.f);

            // Update global state
            state.particles[state.particle_count] = particle;
            state.vel[state.particle_count] = (vel_t){ cosf(angle) * 10, -sinf(angle) * 50 };
            state.particle_count++;
        }

        // Update particle positions
        for (int i = 0; i < state.particle_count; i++)
        {
            state.particles[i].pos[0] += state.vel[i].x * delta;
            state.particles[i].pos[1] += state.vel[i].y * delta;
        }

        pg_update_buffer(instance_buffer, state.particles, state.particle_count);

        // Bind sampler
        pg_bind_sampler(shader, "u_smp", sampler);
        pg_bind_texture(shader, "u_tex", tex);
        pg_set_pipeline(ctx, pipeline);

        // Save current state
        pg_push_state(ctx);
        pg_begin_pass(ctx, NULL, true);

        // Bind buffers and issue a draw command
        pg_bind_buffer(ctx, 0, vertex_buffer);
        pg_bind_buffer(ctx, 1, instance_buffer);

        // Draw using instancing
        pg_draw(ctx, 0, 6, state.particle_count);

        // Clean up
        pg_end_pass(ctx);
        pg_pop_state(ctx);

        // Flush draw commands
        pg_flush(ctx);
    }

    pg_destroy_texture(tex);
    pg_destroy_sampler(sampler);
    pg_destroy_pipeline(pipeline);
    pg_destroy_shader(shader);
    pg_destroy_buffer(vertex_buffer);
    pg_destroy_buffer(instance_buffer);
    pg_destroy_context(ctx);

    pg_shutdown();

    return 0;
}

#undef state //    state_FX
#undef vertex_t // vertex_t_FX

#undef MAX_PARTICLES // (512 * 1024)
#undef NUM_PARTICLES_EMITTED_PER_FRAME // (10)

#undef PI_F // 3.1415927f

#endif // image
