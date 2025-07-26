#define V3_IMPLEMENTATION
#include "v3.h"

#if !V3_RETAIL // by default, microui is only enabled in dev builds

static  char logbuf[64000];
static   int logbuf_updated = 0;
static float bg[3] = { 90, 95, 100 };


static void write_log(const char *text) {
    if (logbuf[0]) { strcat(logbuf, "\n"); }
    strcat(logbuf, text);
    logbuf_updated = 1;
}


static void test_window(void) {
    /* do window */
    if( ui_window("Demo Window", 0/*, mu_rect(40, 40, 300, 450)*/)) {

        /* window info */
        if (ui_section("Window Info",0)) {
            mu_Container *win = mu_get_current_container(ui_ctx);
            mu_layout_row(ui_ctx, 2, (int[]) { 54, -1 }, 0);
            ui_label("Position:");
            ui_label(va("%d, %d", win->rect.x, win->rect.y));
            ui_label("Size:");
            ui_label(va("%d, %d", win->rect.w, win->rect.h));
        }

        /* labels + buttons */
        if (ui_section("Test Buttons",1)) {
            mu_layout_row(ui_ctx, 3, (int[]) { 86, -110, -1 }, 0);
            ui_label("Test buttons 1:");
            if (ui_button("Button 1")) { write_log("Pressed button 1"); }
            if (ui_button("Button 2")) { write_log("Pressed button 2"); }
            ui_label("Test buttons 2:");
            if (ui_button("Button 3")) { write_log("Pressed button 3"); }
            if (ui_button("Popup")) { mu_open_popup(ui_ctx, "Test Popup"); }
            if (ui_popup("Test Popup")) {
                ui_button("Hello");
                ui_button("World");
                ui_popup_end();
            }
        }

        /* tree */
        if (ui_section("Tree and Text", 1)) {
            mu_layout_row(ui_ctx, 2, (int[]) { 140, -1 }, 0);
            mu_layout_begin_column(ui_ctx);
            if (ui_tree("Test 1")) {
                if (ui_tree("Test 1a")) {
                    ui_label("Hello");
                    ui_label("world");
                    ui_tree_end();
                }
                if (ui_tree("Test 1b")) {
                    if (ui_button("Button 1")) { write_log("Pressed button 1"); }
                    if (ui_button("Button 2")) { write_log("Pressed button 2"); }
                    ui_tree_end();
                }
                ui_tree_end();
            }
            if (ui_tree("Test 2")) {
                mu_layout_row(ui_ctx, 2, (int[]) { 54, 54 }, 0);
                if (ui_button("Button 3")) { write_log("Pressed button 3"); }
                if (ui_button("Button 4")) { write_log("Pressed button 4"); }
                if (ui_button("Button 5")) { write_log("Pressed button 5"); }
                if (ui_button("Button 6")) { write_log("Pressed button 6"); }
                ui_tree_end();
            }
            if (ui_tree("Test 3")) {
                static int checks[3] = { 1, 0, 1 };
                ui_bool("Checkbox 1", &checks[0]);
                ui_bool("Checkbox 2", &checks[1]);
                ui_bool("Checkbox 3", &checks[2]);
                ui_tree_end();
            }
            mu_layout_end_column(ui_ctx);

            mu_layout_begin_column(ui_ctx);
            mu_layout_row(ui_ctx, 1, (int[]) { -1 }, 0);
            mu_text(ui_ctx, "Lorem ipsum dolor sit amet, consectetur adipiscing "
                "elit. Maecenas lacinia, sem eu lacinia molestie, mi risus faucibus "
                "ipsum, eu varius magna felis a nulla.");
            mu_layout_end_column(ui_ctx);
        }

        /* background color sliders */
        if (ui_section("Background Color", 1)) {
            mu_layout_row(ui_ctx, 2, (int[]) { -78, -1 }, 74);
            /* sliders */
            mu_layout_begin_column(ui_ctx);
            mu_layout_row(ui_ctx, 2, (int[]) { 46, -1 }, 0);
            ui_float("Red:", &bg[0], 0, 255);
            ui_float("Green:", &bg[1], 0, 255);
            ui_float("Blue:", &bg[2], 0, 255);
            mu_layout_end_column(ui_ctx);
            /* color preview */
            mu_Rect r = mu_layout_next(ui_ctx);
            mu_draw_rect(ui_ctx, r, mu_color(bg[0], bg[1], bg[2], 255));
            char buf[32];
            sprintf(buf, "#%02X%02X%02X", (int) bg[0], (int) bg[1], (int) bg[2]);
            mu_draw_control_text(ui_ctx, buf, r, MU_COLOR_TEXT, MU_OPT_ALIGNCENTER);
        }

        ui_window_end();
    }
}


static void log_window(void) {
    if (ui_window("Log Window", 0/*, mu_rect(350, 40, 300, 200)*/)) {
        /* output text panel */
        mu_layout_row(ui_ctx, 1, (int[]) { -1 }, -25);
        mu_begin_panel(ui_ctx, "Log Output");
        mu_Container *panel = mu_get_current_container(ui_ctx);
        mu_layout_row(ui_ctx, 1, (int[]) { -1 }, -1);
        mu_text(ui_ctx, logbuf);
        mu_end_panel(ui_ctx);
        if (logbuf_updated) {
            panel->scroll.y = panel->content_size.y;
            logbuf_updated = 0;
        }

        /* input textbox + submit button */
        static char buf[128];
        int submitted = 0;
        mu_layout_row(ui_ctx, 2, (int[]) { -70, -1 }, 0);
        if (mu_textbox(ui_ctx, buf, sizeof(buf)) & MU_RES_SUBMIT) {
            mu_set_focus(ui_ctx, ui_ctx->last_id);
            submitted = 1;
        }
        if (ui_button("Submit")) { submitted = 1; }
        if (submitted) {
            write_log(buf);
            buf[0] = '\0';
        }

        ui_window_end();
    }
}




static void style_window(void) {
    static struct { const char *label; int idx; } colors[] = {
        { "text:",         MU_COLOR_TEXT        },
        { "border:",       MU_COLOR_BORDER      },
        { "windowbg:",     MU_COLOR_WINDOWBG    },
        { "titlebg:",      MU_COLOR_TITLEBG     },
        { "titletext:",    MU_COLOR_TITLETEXT   },
        { "panelbg:",      MU_COLOR_PANELBG     },
        { "button:",       MU_COLOR_BUTTON      },
        { "buttonhover:",  MU_COLOR_BUTTONHOVER },
        { "buttonfocus:",  MU_COLOR_BUTTONFOCUS },
        { "base:",         MU_COLOR_BASE        },
        { "basehover:",    MU_COLOR_BASEHOVER   },
        { "basefocus:",    MU_COLOR_BASEFOCUS   },
        { "scrollbase:",   MU_COLOR_SCROLLBASE  },
        { "scrollthumb:",  MU_COLOR_SCROLLTHUMB },
        { NULL }
    };

    if (ui_window("Style Editor", 0/*, mu_rect(350, 250, 300, 240)*/)) {
        for (int i = 0; colors[i].label; i++)
            ui_rgba8(colors[i].label, &ui_ctx->style->colors[i]);
        ui_window_end();
    }
}

int main() {
    for( app_create(.title = "v3", .scale = 0.75 /*,.extra = "GL2"*/); app_swap(); ) {
        style_window();
        log_window();
        test_window();

        glViewport(0, 0, app_width(), app_height());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_BLEND);

        if( strstr(app_state().extra, "GL2") ) {
            glMatrixMode(GL_PROJECTION);
            glRotatef(0.1f, 0, 0, 1);

            glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.6f, -0.75f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.6f, -0.75f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.0f, 0.75f);
            glEnd();
        }

        static int frames = 0, fps = 0, maxfps = 0;
        static double epoch = 0;
        double now = time_ss(), diff = now - epoch; if(diff >= 0.25) fps = frames*(1/(diff + !diff)), frames = 0, epoch = now; else ++frames;
        if( fps > maxfps ) maxfps = fps;
        ui_label(va("%d/%d fps", fps, maxfps));
    }

    return 0;
}

#endif
