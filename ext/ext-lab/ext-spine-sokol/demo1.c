//------------------------------------------------------------------------------
//  spine-switch-skinsets-sapp.c
//  Test if switching skinsets on the same instance works.
//------------------------------------------------------------------------------

#define V3_IMPLEMENTATION
#include "v3.h"

#define init    spine_demo1_init
#define state   spine_demo1_state
#define frame   spine_demo1_frame
#define cleanup spine_demo1_cleanup
#define load_status_t spine_demo1_load_status_t
#define create_spine_objects spine_demo1_create_spine_objects
#define image_data_loaded spine_demo1_image_data_loaded
#define thread_dt spine_demo1_thread_dt

typedef struct {
    bool loaded;
    sspine_range data;
} load_status_t;

#define NUM_SKINSETS (3)
#define NUM_SKINS_PER_SKINSET (8)

static struct {
    sspine_atlas atlas;
    sspine_skeleton skeleton;
    sspine_instance instance;
    sspine_skinset skinsets[NUM_SKINSETS];
    sg_pass_action pass_action;
    struct {
        load_status_t atlas;
        load_status_t skeleton;
    } load_status;
    struct {
        uint8_t atlas[16 * 1024];
        uint8_t skeleton[300 * 1024];
        uint8_t image[512 * 1024];
    } buffers;
} state;

static void image_data_loaded(const void *ptr, int len, void *userdata);
static void create_spine_objects(void);

static const char* skins[NUM_SKINSETS][NUM_SKINS_PER_SKINSET] = {
    {
        "skin-base",
        "accessories/backpack",
        "clothes/dress-blue",
        "eyelids/girly",
        "eyes/eyes-blue",
        "hair/blue",
        "legs/boots-pink",
        "nose/long",
    },
    {
        "skin-base",
        "accessories/bag",
        "clothes/dress-green",
        "eyelids/semiclosed",
        "eyes/green",
        "hair/brown",
        "legs/boots-red",
        "nose/short",
    },
    {
        "skin-base",
        "accessories/cape-blue",
        "clothes/hoodie-blue-and-scarf",
        "eyelids/girly",
        "eyes/violet",
        "hair/pink",
        "legs/pants-green",
        "nose/long",
    }
};

static void init(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });
    sspine_setup(&(sspine_desc){ .logger.func = slog_func });

    state.pass_action = (sg_pass_action){
        .colors[0] = { .load_action = SG_LOADACTION_CLEAR, .clear_value = { 0.0f, 0.0f, 0.0f, 1.0f } }
    };

    {
        file_t f1 = asset("mix-and-match-pma.atlas");
        file_t f2 = asset("mix-and-match-pro.skel");

        state.load_status.atlas = (load_status_t){
            .loaded = true,
            .data = (sspine_range){ .ptr = f1.bin, .size = f1.len }
        };
        state.load_status.skeleton = (load_status_t){
            .loaded = true,
            .data = (sspine_range){ .ptr = f2.bin, .size = f2.len }
        };

        create_spine_objects();
    }

}

static void frame(float delta_time) {
    const float w = app_width();
    const float h = app_height();
    const sspine_layer_transform layer_transform = {
        .size = { .x = w, .y = h },
        .origin = { .x = w * 0.5f, .y = h * 0.5f }
    };

    sspine_update_instance(state.instance, delta_time);
    sspine_draw_instance_in_layer(state.instance, 0);

    sg_begin_pass(&(sg_pass){ .action = state.pass_action, .swapchain = sglue_swapchain() });
    sspine_draw_layer(0, &layer_transform);
    sg_end_pass();
    sg_commit();
}

static void cleanup(void) {
    sspine_shutdown();
    sg_shutdown();
}

static void load_failed(void) {
    state.pass_action.colors[0].clear_value = (sg_color){ 1.0f, 0.0f, 0.0f, 1.0f };
}

static void create_spine_objects(void) {
    // create atlas, skeleton and instance object
    state.atlas = sspine_make_atlas(&(sspine_atlas_desc){
        .data = state.load_status.atlas.data
    });
    assert(sspine_atlas_valid(state.atlas));

    state.skeleton = sspine_make_skeleton(&(sspine_skeleton_desc){
        .atlas = state.atlas,
        .binary_data = state.load_status.skeleton.data
    });

    state.instance = sspine_make_instance(&(sspine_instance_desc){
        .skeleton = state.skeleton
    });
    sspine_set_scale(state.instance, (sspine_vec2){ 0.75f, 0.75f });
    sspine_set_position(state.instance, (sspine_vec2){ 0.0f, 220.0f });
    sspine_set_animation(state.instance, sspine_anim_by_name(state.skeleton, "walk"), 0, true);

    // start loading atlas images
    const int num_images = sspine_num_images(state.atlas);
    for (int img_index = 0; img_index < num_images; img_index++) {
        const sspine_image img = sspine_image_by_index(state.atlas, img_index);
        const sspine_image_info img_info = sspine_get_image_info(img);

        file_t f = asset(img_info.filename.cstr);
        image_data_loaded(f.bin, f.len, (void *)&img);
    }

    // create skinsets
    for (int skinset_index = 0; skinset_index < NUM_SKINSETS; skinset_index++) {
        sspine_skinset_desc desc = {
            .skeleton = state.skeleton
        };
        for (int skin_index = 0; skin_index < NUM_SKINS_PER_SKINSET; skin_index++) {
            desc.skins[skin_index] = sspine_skin_by_name(state.skeleton, skins[skinset_index][skin_index]);
        }
        state.skinsets[skinset_index] = sspine_make_skinset(&desc);
    }

    // set the first skinset as visible
    sspine_set_skinset(state.instance, state.skinsets[0]);
}

static void image_data_loaded(const void *ptr, int size, void *user_data) {
    const sspine_image img = *(sspine_image*)user_data;
    const sspine_image_info img_info = sspine_get_image_info(img);
    assert(img_info.valid);
    if (1) {
        const int desired_channels = 4;
        image_t img = image(.bin = (void*)ptr, .len = size, .n = desired_channels);
        void* pixels = img.pixels;
        int img_width = img.w, img_height = img.h, num_channels = img.n;
        if (pixels) {
            sg_init_image(img_info.sgimage, &(sg_image_desc){
                .width = img_width,
                .height = img_height,
                .pixel_format = SG_PIXELFORMAT_RGBA8,
                .label = img_info.filename.cstr,
                .data.subimage[0][0] = {
                    .ptr = pixels,
                    .size = (size_t)(img_width * img_height * 4)
                }
            });
            sg_init_sampler(img_info.sgsampler, &(sg_sampler_desc){
                .min_filter = img_info.min_filter,
                .mag_filter = img_info.mag_filter,
                .mipmap_filter = img_info.mipmap_filter,
                .wrap_u = img_info.wrap_u,
                .wrap_v = img_info.wrap_v,
                .label = img_info.filename.cstr,
            });
            image_drop(&img);
        } else {
            sg_fail_image(img_info.sgimage);
            load_failed();
        }
    } else if (0) {
        sg_fail_image(img_info.sgimage);
        load_failed();
    }
}

float thread_dt() {
    static threadlocal_ double t = 0;
    double now = time_ss();
    float dt = now - t;
    if(!t) dt = 0;
    t = now;
    return dt;
}

int main() {
    app_create(.title = "spine(skinsets)", "0.50");
    init();
    while(app_swap()) {
        sg_reset_state_cache();

        frame(thread_dt());

        if(ui_button("1")) sspine_set_skinset(state.instance, state.skinsets[0]);
        if(ui_button("2")) sspine_set_skinset(state.instance, state.skinsets[1]);
        if(ui_button("3")) sspine_set_skinset(state.instance, state.skinsets[2]);
    }
    cleanup();
    return 0;
}

#undef init
#undef state
#undef frame
#undef cleanup
#undef load_status_t
#undef create_spine_objects
#undef image_data_loaded
#undef thread_dt
