#include <assert.h>
#include "microui_renderer.h"
#include "microui_atlas.inl"
#include "microui_renderer2.c"
#include "microui_renderer3.c"

void (*r_init)(void);
void (*r_draw_rect)(mu_Rect rect, mu_Color color);
void (*r_draw_text)(const char *text, mu_Vec2 pos, mu_Color color);
void (*r_draw_icon)(int id, mu_Rect rect, mu_Color color);
 int (*r_get_text_width)(const char *text, int len);
 int (*r_get_text_height)(void);
void (*r_set_clip_rect)(mu_Rect rect);
void (*r_clear)(mu_Color clr);
void (*r_present)(void);
void (*r_draw_image)(mu_Rect dst, mu_Rect src, void *texture_handle);


void r_backend(int backend) {
    if( backend == 2 ) {
        r_init = r_init2;
        r_draw_rect = r_draw_rect2;
        r_draw_text = r_draw_text2;
        r_draw_icon = r_draw_icon2;
        r_get_text_width = r_get_text_width2;
        r_get_text_height = r_get_text_height2;
        r_set_clip_rect = r_set_clip_rect2;
        r_clear = r_clear2;
        r_present = r_present2;
        r_draw_image = r_draw_image2;
    }
    if( backend == 3 ) {
        r_init = r_init3;
        r_draw_rect = r_draw_rect3;
        r_draw_text = r_draw_text3;
        r_draw_icon = r_draw_icon3;
        r_get_text_width = r_get_text_width3;
        r_get_text_height = r_get_text_height3;
        r_set_clip_rect = r_set_clip_rect3;
        r_clear = r_clear3;
        r_present = r_present3;
        r_draw_image = r_draw_image3;
    }
}
