#include "ext-microui/api.h"

static mu_Context *ui_ctx;

int ui_window_(const char *label, unsigned flags) {
    static int ox, oy; ONCE ox = 0, oy = -ui_ctx->style->title_height; // ox = app_width() / 2, oy = app_height() / 4;
    static int ow, oh; ONCE ow = app_width() / 5, oh = ow;
    int is_new = !!mu_get_container(ui_ctx, label);
    if( is_new ) /*ox += ui_ctx->style->title_height,*/ oy += ui_ctx->style->title_height;

    if( mu_begin_window_ex(ui_ctx, label, mu_rect(ox,oy, ow,oh), flags) ) {
        mu_layout_row(ui_ctx, 1, (int[]) { -1 }, 0); // 1 column, all space X, 0 auto Y.

        if( is_new ) {
            mu_Container* c = mu_get_current_container(ui_ctx);
            //c->minimized = c->rect.h, c->rect.h = ui_ctx->style->title_height;
        }

        return 1;
    }
    return 0;
}
int ui_window_end_(void) {
#if 1 //< add minimize/maximize button
    mu_Rect bak = mu_get_clip_rect(ui_ctx);
    mu_pop_clip_rect(ui_ctx);

        mu_Container* c = mu_get_current_container(ui_ctx);
        mu_Rect r = c->rect;

        // draw resize icon, bottom-right. fixme: skip block if NORESIZE flag is active
        if( !c->minimized ) {
        r.x += r.w - ui_ctx->style->title_height + 7;
        r.w = ui_ctx->style->title_height;
        r.y += r.h - ui_ctx->style->title_height + 6;
        r.h = ui_ctx->style->title_height;
        mu_draw_icon(ui_ctx, MU_ICON_RESIZE, r, ui_ctx->style->colors[MU_COLOR_TITLETEXT]);
        }

        // draw minimize/maximize buttons, top-right
        r = c->rect;
        r.x += r.w - ui_ctx->style->title_height * 2;
        r.w = ui_ctx->style->title_height;
        r.h = ui_ctx->style->title_height;
#if 0
        // chevrons
        mu_draw_icon(ui_ctx, c->minimized ? MU_ICON_CHEVRON_RIGHT : MU_ICON_CHEVRON_DOWN, r, ui_ctx->style->colors[MU_COLOR_TITLETEXT]);
#elif 1
        // boxes
        mu_Rect box = r;
        if( c->minimized ) {
            box.w /= 3;
            box.h /= 3;
            box.x += box.w;
            box.y += box.h;
            box.x += 6;
        } else {
            box.w /= 2;
            box.x += box.w;
            box.y += box.h/1.5 - 1;
            box.h = 1;
            box.w -= 2;
        }
        mu_draw_box(ui_ctx, box, ui_ctx->style->colors[MU_COLOR_TITLETEXT]);
#endif

        //mu_draw_rect(ui_ctx, r, mu_color(255,255,0,255)); // debug hovering area
        //mu_draw_icon(ui_ctx, MU_ICON_ATLAS, r, ui_ctx->style->colors[MU_COLOR_TITLETEXT]); // debug icons
        if( ui_ctx->mouse_pressed == MU_MOUSE_LEFT && mu_mouse_over(ui_ctx, r) ) {
            if( !c->minimized )
                c->minimized = c->rect.h, c->rect.h = ui_ctx->style->title_height;
            else
                c->rect.h = c->minimized, c->minimized = 0;
        }

    mu_push_clip_rect(ui_ctx,bak);
#endif
    return mu_end_window(ui_ctx), 1;
}

int ui_tree_(const char *label) {
    return mu_begin_treenode(ui_ctx, label);
}
int ui_tree_end_(void) {
    return mu_end_treenode(ui_ctx), 1;
}

int ui_popup_(const char *label) {
    return mu_begin_popup(ui_ctx, label);
}
int ui_popup_end_(void) {
    return mu_end_popup(ui_ctx), 1;
}

int ui_label_(const char *txt) {
//    return mu_label(ui_ctx, txt), 0;
    return mu_text(ui_ctx,txt), 0;
}
int ui_button_(const char *txt) {
    return mu_button(ui_ctx, txt);
}
int ui_buttons_(int num, ...) {
    /**/ if( num <= 1 ) mu_layout_row(ui_ctx, 1, (int[]) { -1 }, 0);
    else if( num == 2 ) mu_layout_row(ui_ctx, 2, (int[]) { -50, -1 }, 0);
    else if( num == 3 ) mu_layout_row(ui_ctx, 3, (int[]) { 50, 50, -1 }, 0);
    else if( num == 4 ) mu_layout_row(ui_ctx, 4, (int[]) { 50, 50, 50, -1 }, 0);
    else if( num == 5 ) mu_layout_row(ui_ctx, 5, (int[]) { 50, 50, 50, 50, -1 }, 0);
    else if( num == 6 ) mu_layout_row(ui_ctx, 6, (int[]) { 50, 50, 50, 50, 50, -1 }, 0);
    else die("ui_buttons: unsupported num of buttons");

    int choice = 0;
    va_list v;
    va_start(v,num);
    for( int i = 0; i < num; ++i ) {
        const char *lbl = va_arg(v, const char *);
        if( ui_button_(lbl) ) choice = i+1;
    }
    va_end(v);
    return choice;
}
int ui_section_(const char *txt, int open) {
    return mu_header_ex(ui_ctx, txt, !!open * MU_OPT_EXPANDED);
}
int ui_float_(const char *txt, float *f, float minf, float maxf) {
    //mu_layout_row(ui_ctx, 2, (int[]) { 50, -1 }, 0);
    //mu_layout_row(ui_ctx, 2, (int[]) { -200, -1 }, 0);
    return ui_label_(txt), mu_slider(ui_ctx, f, minf, maxf);
}
int ui_bool_(const char *txt, void *b) {
    STATIC_ASSERT(sizeof(bool) <= sizeof(int));
    return mu_checkbox(ui_ctx, txt, (int*)b);
}

static int uint8_slider(mu_Context *ui_ctx, unsigned char *value, int low, int high) {
    static float tmp;
    mu_push_id(ui_ctx, &value, sizeof(value)); //< needed? also, shouldnt it be just `value, sizeof(value)`?
    tmp = *value;
    int res = mu_slider_ex(ui_ctx, &tmp, low, high, 0, "%.0f", MU_OPT_ALIGNCENTER);
    *value = tmp;
    mu_pop_id(ui_ctx);
    return res;
}
int ui_rgba8_(const char *txt, void *f) {
    unsigned char *f8 = (unsigned char*)f;
    int sw = mu_get_current_container(ui_ctx)->body.w * 0.14;
    mu_layout_row(ui_ctx, 6, (int[]) { 80, sw, sw, sw, sw, -1 }, 0);
    mu_label(ui_ctx, txt);
    int rc = 0;
    rc |= uint8_slider(ui_ctx, f8+0, 0, 255);
    rc |= uint8_slider(ui_ctx, f8+1, 0, 255);
    rc |= uint8_slider(ui_ctx, f8+2, 0, 255);
    rc |= uint8_slider(ui_ctx, f8+3, 0, 255);
    mu_Color col = { f8[0], f8[1], f8[2], f8[3] }; // r.g.b.a
    mu_draw_rect(ui_ctx, mu_layout_next(ui_ctx), col );
    return rc;
}

int ui_hovered_(void) {
    return !!ui_ctx->hover_root;
}

static int text_width(mu_Font font, const char *text, int len) {
    RGFW_UNUSED(font);
    if (len < 0) { len = strlen(text); }
    return r_get_text_width(text, len);
}

static int text_height(mu_Font font) {
    RGFW_UNUSED(font);
    return r_get_text_height();
}

static
void ui_init_() {
    ONCE {
        int use_backend = strstri(app_state().extra, "GL2") ? 2 : 3;
        lputs(va("microui GL%d", use_backend));
        r_backend( use_backend );

        // init microui renderer
        // r_init();

        // init microui
        mu_Context *ctx = (mu_Context*)malloc(sizeof(mu_Context));
        mu_init(ui_ctx = ctx);
        ui_ctx->text_width = text_width;
        ui_ctx->text_height = text_height;

        // cherry titlebar bg
        ui_ctx->style->colors[MU_COLOR_TITLEBG].r = 115;
        ui_ctx->style->colors[MU_COLOR_TITLEBG].g = 0;
        ui_ctx->style->colors[MU_COLOR_TITLEBG].b = 50;
        // white-yellow titlebar fg
        ui_ctx->style->colors[MU_COLOR_TITLETEXT].r = 240;
        ui_ctx->style->colors[MU_COLOR_TITLETEXT].g = 230;
        ui_ctx->style->colors[MU_COLOR_TITLETEXT].b = 200;
        // swap scrollbar colors, so the grabbing area is brighter
        mu_Color *arr = ui_ctx->style->colors, swap;
        swap = arr[MU_COLOR_SCROLLBASE];
        arr[MU_COLOR_SCROLLBASE] = arr[MU_COLOR_SCROLLTHUMB];
        arr[MU_COLOR_SCROLLTHUMB] = swap;
    }
}

static
void ui_begin_(void *arg) { // must be initialized after window is created
    ui_init_();

    static const char key_map[256] = {
        [ (RGFW_mouseLeft + 1)   & 0xff ] = MU_MOUSE_LEFT,
        [ (RGFW_mouseRight + 1)  & 0xff ] = MU_MOUSE_RIGHT,
        [ (RGFW_mouseMiddle + 1) & 0xff ] = MU_MOUSE_MIDDLE,
        [ RGFW_shiftL            & 0xff ] = MU_KEY_SHIFT,
        [ RGFW_shiftR            & 0xff ] = MU_KEY_SHIFT,
        [ RGFW_controlL          & 0xff ] = MU_KEY_CTRL,
        [ RGFW_controlR          & 0xff ] = MU_KEY_CTRL,
        [ RGFW_altL              & 0xff ] = MU_KEY_ALT,
        [ RGFW_altR              & 0xff ] = MU_KEY_ALT,
        [ RGFW_return            & 0xff ] = MU_KEY_RETURN,
        [ RGFW_backSpace         & 0xff ] = MU_KEY_BACKSPACE,
    };

    /* handle RGFW events */
    extern int text[], text_count, up[], up_count, down[], down_count;

    int mx = mouse('x'), my = mouse('y'), mw = mouse('w');

    mu_input_mousemove(ui_ctx, mx, my);
    mu_input_scroll(ui_ctx, 0, (mw / 100.) * -30);

    for( int i = 0; i < up_count; ++i ) {
        int c = up[i] & 0xff;
        /**/ if( c > 3 ) mu_input_keyup(ui_ctx, key_map[c]);
        else if( c > 0 ) mu_input_mouseup(ui_ctx, mx, my, key_map[c]);
    }
    for( int i = 0; i < down_count; ++i ) {
        int c = down[i] & 0xff;
        /**/ if( c > 3 ) mu_input_keydown(ui_ctx, key_map[c]);
        else if( c > 0 ) mu_input_mousedown(ui_ctx, mx, my, key_map[c]);
    }
    for( int i = 0; i < text_count; ++i ) {
        char str[2] = { text[i], '\0' };
        mu_input_text(ui_ctx, str);
    }

    // r_clear(mu_color(255,0,255, 255));

    mu_begin(ui_ctx);

    ui_window_("debug", MU_OPT_NOCLOSE);
}

static
void ui_end_(void *arg) {
    ui_window_end_(); // "debug" window

    mu_end(ui_ctx);

    // render microui
    for( mu_Command *cmd = NULL; mu_next_command(ui_ctx, &cmd); ) {
        switch (cmd->type) {
            case MU_COMMAND_TEXT: r_draw_text(cmd->text.str, cmd->text.pos, cmd->text.color); break;
            case MU_COMMAND_RECT: r_draw_rect(cmd->rect.rect, cmd->rect.color); break;
            case MU_COMMAND_ICON: r_draw_icon(cmd->icon.id, cmd->icon.rect, cmd->icon.color); break;
            case MU_COMMAND_CLIP: r_set_clip_rect(cmd->clip.rect); break;
        }
    }
    r_present();
}

AUTORUN {
    on_frame_begin(ui_begin_, ui_ctx);
    on_frame_end(ui_end_, ui_ctx);
}
