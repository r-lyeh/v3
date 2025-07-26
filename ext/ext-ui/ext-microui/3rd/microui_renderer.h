#ifndef MICROUI_RENDERER_H
#define MICROUI_RENDERER_H

#include "microui.h"

extern void (*r_init)(void);
extern void (*r_draw_rect)(mu_Rect rect, mu_Color color);
extern void (*r_draw_text)(const char *text, mu_Vec2 pos, mu_Color color);
extern void (*r_draw_icon)(int id, mu_Rect rect, mu_Color color);
extern  int (*r_get_text_width)(const char *text, int len);
extern  int (*r_get_text_height)(void);
extern void (*r_set_clip_rect)(mu_Rect rect);
extern void (*r_clear)(mu_Color color);
extern void (*r_present)(void);
extern void (*r_draw_image)(mu_Rect dst, mu_Rect src, void *texture_handle);

void r_backend(int backend);

#endif

