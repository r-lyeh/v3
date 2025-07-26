// -----------------------------------------------------------------------------
// immediate ui framework
// - rlyeh, public domain
//
// @fixme: tooltips in toolbars
// @fixme: docking colors
// @fixme: panel inside window
// @fixme: nest ui2_browser() within a window or within a panel
// @fixme: do malloc() ttf contents before passing them to imgui. also cfg->FontDataOwnedByAtlas = true
// @fixme: UI2_LABEL color bar in ui2_list()
// @fixme: return value of ui2_image()/ui2_tileset() not pixel perfect
//
// @todo: ui2_gallery()
// @todo: subwindows
// @todo: tribool: int flags = 2, maxflags = 3; igCheckboxFlags_IntPtr( "Tribool", &flags, maxflags );

#if !CODE

enum UI2_FLAGS { // for windows and panels
    UI2_OPEN = 1,
    UI2_MENUS = 2,
    UI2_CENTER = 4,
    UI2_TRANSPARENT = 8,
    UI2_FULLSCREEN = 16,
};

API int ui2_notify(const char *label, const char *body);
API int ui2_window(const char *label, int *flags);
API int   ui2_panel(const char *label, int flags); // can be nested within a window, or freestanding
API int     ui2_section(const char *label);
API int     ui2_collapse(const char *label, const char *id);
API int       ui2_separator(const char *label); // can be NULL
API int       ui2_bool(const char *label, bool *value);
API int       ui2_int(const char *label, int value[1]);
API int       ui2_int2(const char *label, int value[2]);
API int       ui2_int3(const char *label, int value[3]);
API int       ui2_clampi(const char *label, int *value, int mini, int maxi);
API int       ui2_unsigned(const char *label, unsigned value[1]);
API int       ui2_unsigned2(const char *label, unsigned value[2]);
API int       ui2_unsigned3(const char *label, unsigned value[3]);
API int       ui2_text(const char *label, unsigned lines, char **text);
API int       ui2_string(const char *label, char **string);
API int       ui2_buffer(const char *label, char *buffer, int buflen);
API int       ui2_float(const char *label, float value[1]);
API int       ui2_float2(const char *label, float value[2]);
API int       ui2_float3(const char *label, float value[3]);
API int       ui2_float4(const char *label, float value[4]);
API int       ui2_floatabs(const char *label, float value[1]);
API int       ui2_clampf(const char *label, float *value, float minf, float maxf);
API int       ui2_ivec2(const char *label, ivec2 *v);
API int       ui2_ivec3(const char *label, ivec3 *v);
API int       ui2_vec2(const char *label, vec2 *v);
API int       ui2_vec3(const char *label, vec3 *v);
API int       ui2_vec4(const char *label, vec4 *v);
API int       ui2_mat33(const char *label, float M[9]);
API int       ui2_mat34(const char *label, float M[12]);
API int       ui2_mat44(const char *label, float M[16]);
API int       ui2_color3(const char *label, unsigned *color); //[0..255]
API int       ui2_color3f(const char *label, float color[3]); //[0..1]
API int       ui2_color4(const char *label, unsigned *color); //[0..255]
API int       ui2_color4f(const char *label, float color[4]); //[0..1]
API int       ui2_button(const char *label);
API int       ui2_buttons(int num_buttons, /*labels*/...);
API int       ui2_toolbar(const char *label, int num_icons, /*icon labels*/...); // returns 0 (no opt), or [1..N] option
API int       ui2_radio(const char *label, int *selected, int num_icons, /*icon labels*/...); // returns 0 (no opt), or [1..N] option
API int       ui2_alert(const char *message, int *show);
API int       ui2_dialog(const char *label, const char *text, int choices);
API int       ui2_list(const char *label, int *selected, int num_items, const char **items);
API int       ui2_bitmask(const char *label, int bits, void *ptr);
API int       ui2_label(const char *label);
API int       ui2_label2(const char *label, const char *text);
API int       ui2_slider(const char *label, float *value);
API int       ui2_slider2(const char *label, float *value, const char *caption);
API int       ui2_image(const char *label, unsigned id, unsigned w, unsigned h); // (-1,-1) to use (avail_width,line_height) size. returns XXYY percentages when clicked
API int       ui2_subimage(const char *label, unsigned id, unsigned iw, unsigned ih, float u0, float v0, float u1, float v1); // returns XXYY percentages when clicked
API int       ui2_percent(const char *label, float value); // use [0..1], [0..100], [0..1000] or NaN
API int       ui2_link(const char *label, const char *url);
API int     ui2_collapse_end();
API int   ui2_panel_end();
API int   ui2_tray(); // can be nested within a window (local tray), or freestanding (desktop tray)
API int     ui2_menu(const char *label); // must be nested within tray
API int       ui2_case(const char *label, bool disabled); // must be nested within menu
API int     ui2_menu_end();
API int   ui2_tray_end();
API int ui2_window_end();

API int   ui2_print(const char *title, const char *text); // printfs to a console window. \b clears log
API char* ui2_console(const char *title); // displays console. wont create a `title` window if NULL. returns sent command or NULL if none

// demo
API int ui2_demo(int do_windows);

// utils
API int ui2_enable(bool);
API int ui2_enabled();

API int ui2_hide(bool);
API int ui2_hidden();

API int ui2_active();
API int ui2_hovered();

API int ui2_window_show(const char *name, int vis);
API int ui2_window_shown(const char *name);

// internals
API int   ui2_loadfont(unsigned slot, float pt, float brightness, const char *resource, const void *runes);
API int   ui2_loadfonts();
API void**ui2_fonthandles(unsigned *count);



// macros

#define ui2_array(label,type,ptr) do { \
    int changed = 0; \
    if( ui2_collapse(label, va(#type "%p",ptr)) ) { \
        char label_ex[8]; \
        for( int idx = 0, iend = array_count(*(ptr)); idx < iend; ++idx ) { \
            type* it = *(ptr) + idx; \
            snprintf(label_ex, sizeof(label_ex), "[%d]", idx); \
            changed |= ui2_##type(label_ex, it); \
        } \
        ui2_collapse_end(); \
    } \
} while(0)

#else
#include <float.h> // FLT_MIN,FLT_MAX

int igTextLineHeight() {
    return igGetFontSize() + igGetStyle()->FramePadding.y * 2; 
}
int igButton2(const char *text, ImVec2 align) {
    char *tooltip = strchr(text, '@');
    if( !tooltip ) return igButton(text, align);
    static char label[256];
    snprintf(label, 256, "%.*s", (int)(tooltip - text), text);
    int ret = igButton(label, align);
    if( igIsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled) )
        igSetTooltip(tooltip+1);
    return ret;
}

int ui2_hue = 0;
int ui2_debug = 1;
uint64_t ui2_frame = 0;
unsigned ui2_hue32;

ImFont *ui2_fonts[4] = {0};

void** ui2_fonthandles(unsigned *count) {
    if( count ) {
        *count = 0;
        for( int i = 0; i < countof(ui2_fonts); ++i ) *count += !!ui2_fonts[i];
    }
    return ui2_fonts;
}

const
int ui2_slider_flags = 0;
// ImGuiSliderFlags_Logarithmic = 1 << 5,
// ImGuiSliderFlags_NoRoundToFormat = 1 << 6,
// ImGuiSliderFlags_NoInput = 1 << 7,
// ImGuiSliderFlags_WrapAround = 1 << 8,
// ImGuiSliderFlags_ClampOnInput = 1 << 9,
// ImGuiSliderFlags_ClampZeroRange = 1 << 10,
// ImGuiSliderFlags_AlwaysClamp = ImGuiSliderFlags_ClampOnInput | ImGuiSliderFlags_ClampZeroRange,

char *ui2_filter;
void ui2_filterbar()
{
    static int location = 0<<1|1<<0; // top-right
    ImGuiIO* io = igGetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings /*| ImGuiWindowFlags_NoFocusOnAppearing*/ | ImGuiWindowFlags_NoNav;
    ImGuiViewport* viewport = igGetMainViewport();
    if (location >= 0) {
        const float PAD = 10.0f;
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        igSetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        igSetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    // igSetNextWindowBgAlpha(0.35f); // Transparent background
    if (igBegin("##SearchFilter", NULL, window_flags)) {
        char *bak = ui2_filter; ui2_filter = 0;
        ui2_string(UI2_ICON(SEARCH) "Filter", &bak);
        ui2_filter = bak;
    }
    igEnd();
}


int ui2_global_id;
void ui2_tick() {
    // new frame
    ui2_frame++;

    // restart the global ID counter
    ui2_global_id = 0;

    // reset hue
    ui2_hue = 0;
    ImVec4 color; ui2_hue32 = igGetColorU32_Vec4((ImColor_HSV((ImColor*)&color, ui2_hue / 7.0f, 0.7f, 0.7f, 1), color));

    // this special panel comes first. it has top priority in panels stack
    if( ui2_debug )
    ui2_panel("Debug##Default",0) && ui2_panel_end(); // this one comes first

    // display filter toolbar if requested
    static bool filter_open = 0;
    if( key_down('f') && key_held(KEY_LCTRL) ) {
        filter_open ^= 1;
    }
    if( key_down(KEY_ESC) ) { // escape
        filter_open = 0;
    }

    if( filter_open ) {
        ui2_filterbar();
    } else {
        if( ui2_filter ) FREE( ui2_filter );
        ui2_filter = 0;
    }
}



typedef map_(int,int*) windows_t;
static windows_t *windows;
int ui2_window_shown(const char *name) {
    windows_t *found = map_find(windows, quark(name));
    return found ? !!(*found->value & UI2_OPEN) : 0;
}
int ui2_window_show(const char *name, int vis) {
    windows_t *found = map_find(windows, quark(name));
    if( found ) { if( vis ) *found->value |= UI2_OPEN; else *found->value &= ~UI2_OPEN; }
    return !!vis;
}
int ui2_window(const char *label, int *flags) {
    assert( flags );
    windows_t *found = map_find_or_add(windows, quark(label));
    found->value = flags;

    bool wopen = !!(*flags & UI2_OPEN);
    if( wopen ) {
        igPushID_Int(++ui2_global_id);

        igSetNextWindowSize(ImVec2(250,1.5*250), ImGuiCond_FirstUseEver);

        if(*flags & UI2_OPEN/*UI2_CENTER*/) {
        ImVec2 center = {0};
        ImGuiViewport_GetCenter(&center, igGetMainViewport());
        igSetNextWindowPos(center, ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
        }

        int igFlags = (*flags & UI2_MENUS ? ImGuiWindowFlags_MenuBar:0); // |ImGuiWindowFlags_AlwaysAutoResize |ImGuiWindowFlags_NoCollapse;
        if(*flags & UI2_FULLSCREEN) {
            igFlags |= ImGuiWindowFlags_NoDecoration;
            igFlags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

            ImGuiViewport* viewport = igGetMainViewport();
            igSetNextWindowPos(viewport->WorkPos, ImGuiCond_Always, ImVec2(0,0));
            igSetNextWindowSize(viewport->WorkSize, ImGuiCond_Always);
        }
        if(*flags & UI2_TRANSPARENT) {
            igFlags |= ImGuiWindowFlags_NoDecoration;
            igFlags |= ImGuiWindowFlags_NoBackground;
            igFlags |= ImGuiWindowFlags_NoTitleBar;
            igFlags |= ImGuiWindowFlags_NoFocusOnAppearing;
            igFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        }

        igBegin(label, &wopen, igFlags);
        //igPushItemWidth(-100); // fill the space and leave 100 pixels for the label
        igPushItemWidth(-FLT_MIN); // remove the blank space typically used for the labels
        *flags = (*flags & ~UI2_OPEN) | (wopen & UI2_OPEN);
        return 1;
    }
    return 0;
}
int ui2_window_end() {
    igEnd();
    igPopID();
    return 1;
}

typedef set_(int) labels_t;
static labels_t set;

static
int ui2_adjust_nextpanel(const char *label, int flags) {
    // postpone panel creation a few frames. to let main menubar be settled.
    if( ui2_frame <= 2 ) return 0;
    // postpone open panel creation to last place in stack. it will be handled within next frame.
    if( ui2_frame == 3 && (flags & UI2_OPEN) ) return 0;

    static labels_t *set = 0;
    labels_t *found = set_find(set, quark(label)); int first = !found;
    if( !found ) set_insert(set, quark(label));

    if( first ) {
        ImGuiViewport* viewport = igGetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot = ImVec2(0,0);

        bool has_menubar = !!work_pos.y;
        if( !has_menubar ) {
            igGetStyle()->DisplayWindowPadding.y = 0;
            igGetStyle()->DisplaySafeAreaPadding.y = 0;
        }

        int spacing = 16;
        static int items = 0;
        const vec2 PAD = has_menubar ? vec2(2,5) : vec2(0,0);
        window_pos.x = (work_pos.x + PAD.x);
        window_pos.y = (work_pos.y + PAD.y) + (spacing * items++);
        //printf("%s: window_pos(%f,%f) work_pos(%f,%f) work_size(%f,%f)\n", label, window_pos.x,window_pos.y, work_pos.x,work_pos.y, work_size.x,work_size.y);
        igSetNextWindowPos(window_pos, ImGuiCond_Once, window_pos_pivot);
        igSetNextWindowSize(ImVec2(work_size.x*0.33,work_size.y-window_pos.y+viewport->WorkPos.y), ImGuiCond_Once);
        igSetNextWindowViewport(viewport->ID);
        igSetNextWindowCollapsed(flags & UI2_OPEN ? 0 : 1, ImGuiCond_Once);
    }

    if(flags & UI2_CENTER) {
    ImVec2 center = {0};
    ImGuiViewport_GetCenter(&center, igGetMainViewport());
    igSetNextWindowPos(center, ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    }

    return 1;
}

int ui2_panel(const char *label, int flags) {// may be embedded inside a window, or standalone
    if(!ui2_adjust_nextpanel(label, flags)) return 0;

    igPushID_Int(++ui2_global_id);

    igBegin(label, NULL, 0|ImGuiWindowFlags_NoDocking|(flags & UI2_MENUS ? ImGuiWindowFlags_MenuBar:0));
    //igPushItemWidth(-100); // fill the space and leave 100 pixels for the label
    igPushItemWidth(-FLT_MIN); // remove the blank space typically used for the labels
    return 1;
}
int ui2_panel_end() {
    igEnd();
    igPopID();
    return 1;
}



int ui2_notify(const char *label, const char *body) {
    int type = 'i'; // (i)nfo, o(k), (e)rror, (w)arning types
    /**/ if( strstri(label, "warn") ) type = 'w';
    else if( strstri(label, "err") || strstri(label,"fail") ) type = 'e';
    else if( strstri(label, "succ") || strstri(label,"complet") ) type = 'k';
    else if( strstri(label, "random") ) type = "weik"[rand()%4];
    return igInsertNotification(type, label, body, strchr("ki",type) ? 3 : -1), 1;
}


int ui2_tray_global = 1;
int ui2_tray() {
    ui2_tray_global = igCurrentWindowStackSize() <= 1;
    return ui2_tray_global ? igBeginMainMenuBar() : igBeginMenuBar();
}
int ui2_tray_end() {
    ui2_tray_global ? igEndMainMenuBar() : igEndMenuBar();

    return 1;
}
int ui2_menu(const char *label) {
    bool enabled = 1;
    return igBeginMenu(label, enabled);
}
int ui2_menu_end() {
    return igEndMenu(), 1;
}
int ui2_case(const char *label, bool disabled) {
    const char *icon = NULL;
    const char *shortcut = NULL;
    bool enabled = !disabled;
    bool selected = false;
    return igMenuItemEx(label,icon,shortcut,selected,enabled);
}


/// [src] https://github.com/ocornut/imgui/issues/3565#issuecomment-2120416586 by azmr
/// Make contents in a table cover the entire row rather than just a single column.
///
/// Always covers the entire row (not just the remaining columns);
/// can sort of coexist with per-column data, but may not be as intended.
/// Accounts for:
/// - scrollbar
/// - header row
/// - column reordering
/// - hidden columns
float igTableFullRowBegin() {
    ImGuiTable *table = igGetCurrentTable();

    // Set to the first visible column, so that all contents starts from the leftmost point
    for (ImGuiTableColumnIdx *clmn_idx = table->DisplayOrderToIndex.Data,
         *end = table->DisplayOrderToIndex.DataEnd;
         clmn_idx < end; ++clmn_idx)
    {   if (igTableSetColumnIndex(*clmn_idx)) break;   }

    ImRect *work_rect    = &igGetCurrentWindow()->WorkRect;
    float   restore_x    = work_rect->Max.x;
    ImRect  bg_clip_rect = table->BgClipRect; // NOTE: this accounts for header column & scrollbar

    igPushClipRect(bg_clip_rect.Min, bg_clip_rect.Max, 0); // ensure that both our own drawing...
    work_rect->Max.x = bg_clip_rect.Max.x;                 // ...and Dear ImGui drawing will be visible across the entire row

    return restore_x;
}
void igTableFullRowEnd(float restore_x) {
    igGetCurrentWindow()->WorkRect.Max.x = restore_x;
    igPopClipRect();
}

// complex helper function, that
// 1) creates color bar on left side, as long as label is not null
// 2) appends then some text, as long as label is not empty ""
// 3) highlights and indents such text when mouse is hovering
// 4) creates a tooltip after, as long as label contains a '@text' within
// 5) filters out contents by early exiting, as long as label and ui2_filter mismatch
// historically, we used to have also '<=>aligning', '*bolditalic', '[emoji]text'
// label color bar:
static bool   UI2_LABEL_USE_COLOR, UI2_LABEL_HOVERED;
static ImVec2 UI2_LABEL_MIN, UI2_LABEL_MAX;
//
static float UI2_LABEL_RESTORE;
static int UI2_LABEL_1OF2(const char *label) {
    enum { BAR_MIN_WIDTH = 2, BAR_MAX_WIDTH = 4, BAR_HEIGHT = 2 };
    enum { BOLD_INDENT = 10, BOLD_ALPHA = 150, BOLD_COLOR = ImGuiCol_Text };
    enum { TABLE_FLAGS = ImGuiTableFlags_Resizable }; // |ImGuiTableFlags_SizingStretchProp|ImGuiTableFlags_ContextMenuInBody|ImGuiTableFlags_NoHostExtendX };

    if( label && ui2_filter && ui2_filter[0] ) if( !strstri(label, ui2_filter) ) return 0;

    const char *tooltip = 0;
    int has_label = label && label[0]; UI2_LABEL_USE_COLOR = !!label, UI2_LABEL_HOVERED = 0;
    if(!igBeginTable("##", 2, TABLE_FLAGS, ImVec2(0,0), 0.f)) 
        return 0;

    igTableNextColumn();

    if(!has_label) label = "";

        static char copy[128];
        tooltip = strrchr(label, '@');
        if( tooltip ) snprintf(copy, 128, "%.*s %s", (int)(tooltip - label), label, UI2_ICON(INFO/*HELP*/) );

        igTextWithHoverColor(igGetColorU32_Col(BOLD_COLOR, BOLD_ALPHA / 100.f ), ImVec2(BOLD_INDENT/2,BOLD_INDENT), tooltip ? copy : label);
        UI2_LABEL_HOVERED = igIsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled);

        igGetItemRectMin(&UI2_LABEL_MIN); UI2_LABEL_MIN.y -= BAR_HEIGHT;

        if( tooltip && UI2_LABEL_HOVERED ) igSetTooltip("%s", tooltip + 1);

    igTableNextColumn();
    igPushItemWidth(-1);

    if(!has_label) UI2_LABEL_RESTORE = igTableFullRowBegin();
    return 1;
}
static int UI2_LABEL_2OF2( int rc ) {
    enum { BAR_MIN_WIDTH = 2, BAR_MAX_WIDTH = 4, BAR_HEIGHT = 2-1 };
    if( UI2_LABEL_RESTORE ) igTableFullRowEnd(UI2_LABEL_RESTORE), UI2_LABEL_RESTORE = 0;
    igPopItemWidth(), igEndTable();

    if( 1 ) {
        igGetItemRectMax(&UI2_LABEL_MAX); UI2_LABEL_MAX.y += BAR_HEIGHT;
        igRenderFrame(UI2_LABEL_MIN,ImVec2(UI2_LABEL_MIN.x+(UI2_LABEL_HOVERED ? BAR_MAX_WIDTH : BAR_MIN_WIDTH),UI2_LABEL_MAX.y),UI2_LABEL_USE_COLOR ? ui2_hue32 : 0,false/*borders*/,0/*rounding*/);
    }

    return rc;
}
#define UI2_LABEL(label, ...) (UI2_LABEL_1OF2(label) ? UI2_LABEL_2OF2(__VA_ARGS__) : 0)


int ui2_label(const char *label) {
    return UI2_LABEL(label, (igDummy(ImVec2(1,1)), 1));
}
int ui2_label2(const char *label, const char *text) {
    return UI2_LABEL(label, (igText("%s", text), 1));
}

struct InputTextCallback_UserData {
    char*                  Str;
};
static int InputTextCallback(ImGuiInputTextCallbackData* data) {
    struct InputTextCallback_UserData* user_data = (struct InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        data->Buf = user_data->Str = REALLOC(user_data->Str, data->BufTextLen + 1); // @fixme: +1 needed?
    }
    return 0;
}
int ui2_string(const char *label, char **str) {
    if(!UI2_LABEL_1OF2(label)) return 0;

    strcatf(str, "", "");

    struct InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = *str;
    int rc = igInputText(label, *str, strlen(*str)+1, ImGuiInputTextFlags_CallbackCharFilter|ImGuiInputTextFlags_CallbackResize, InputTextCallback, &cb_user_data);
    *str = cb_user_data.Str;

    return UI2_LABEL_2OF2(rc);
}
int ui2_text(const char *label, unsigned lines, char **str) {
    if(!UI2_LABEL_1OF2(label)) return 0;

    strcatf(str, "", "");

    struct InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = *str;
    int rc = igInputTextMultiline(label, *str, strlen(*str)+1, ImVec2(-1,igTextLineHeight()*(lines+!lines)), ImGuiInputTextFlags_CallbackCharFilter|ImGuiInputTextFlags_CallbackResize, InputTextCallback, &cb_user_data);
    *str = cb_user_data.Str;

    return UI2_LABEL_2OF2(rc);
}
int ui2_buffer(const char *label, char *buffer, int buflen) {
    return UI2_LABEL(label, igInputText("##", buffer, buflen, 0/*ImGuiInputTextFlags*/, NULL, NULL));
}

int ui2_clampi(const char *label, int *v, int mini, int maxi) {
    //int range[] = { mini, maxi };
    //return UI2_LABEL(label, igDragScalarN("##", ImGuiDataType_S32,v,1,1.f,range+0,range+1,NULL,0));
    return UI2_LABEL(label, igSliderInt("##", v, mini, maxi, "%d", ui2_slider_flags));
}
int ui2_clampf(const char *label, float *v, float minf, float maxf) {
    //float range[] = { minf, maxf };
    //return UI2_LABEL(label, igDragScalarN("##", ImGuiDataType_Float,v,1,1.f,range+0,range+1,NULL,0));
    return UI2_LABEL(label, igSliderFloat("##", v, minf, maxf, "%.3f", ui2_slider_flags));
}
int ui2_int(const char *label, int v[1]) {
    // return UI2_LABEL(label, igInputInt("##", v, 1, 10, 0));
    return UI2_LABEL(label, igDragScalarN("##",ImGuiDataType_S32,v,1,1.f,NULL,NULL,NULL,0));
}
int ui2_int2(const char *label, int v[2]) {
    return UI2_LABEL(label, igDragScalarN("##",ImGuiDataType_S32,v,2,1.f,NULL,NULL,NULL,0));
}
int ui2_int3(const char *label, int v[3]) {
    return UI2_LABEL(label, igDragScalarN("##",ImGuiDataType_S32,v,3,1.f,NULL,NULL,NULL,0));
}
int ui2_unsigned(const char *label, unsigned v[1]) {
    return UI2_LABEL(label, igDragScalarN("##",ImGuiDataType_U32,v,1,1.f,NULL,NULL,NULL,0));
}
int ui2_unsigned2(const char *label, unsigned v[2]) {
    return UI2_LABEL(label, igDragScalarN("##",ImGuiDataType_U32,v,2,1.f,NULL,NULL,NULL,0));
}
int ui2_unsigned3(const char *label, unsigned v[3]) {
    return UI2_LABEL(label, igDragScalarN("##",ImGuiDataType_U32,v,3,1.f,NULL,NULL,NULL,0));
}

int ui2_bool(const char *label, bool *value) {
    return UI2_LABEL(label, igCheckbox("##", value));
}
int ui2_percent(const char *label, float pct) {
    if( pct != pct ) return UI2_LABEL(label, (igProgressBar(-1.0f * igGetTime(),ImVec2(-1,0),NULL),0));
    if( pct > 100 ) return ui2_percent(label, pct / 1000);
    if( pct > 1 ) return ui2_percent(label, pct / 100);
    return UI2_LABEL(label, (igProgressBar(pct,ImVec2(-1,0),NULL),0));
}
int ui2_floatabs(const char *label, float *v) {
    int rc = ui2_float(label, v); if( *v < 0 ) *v = 0;
    return rc;
}
int ui2_float(const char *label, float *v) {
//    return UI2_LABEL(label, igDragFloat("##", v, 0.005f, 0.0f, 1.0f, "%.2f", 0/*ImGuiSliderFlags_AlwaysClamp*/));
//    float zero = 0, one = 1; return UI2_LABEL(label, igSliderScalar("##", ImGuiDataType_Float, v, &zero, &one));
    return UI2_LABEL(label, igDragFloat("##", v, 0.005f, 0, 0, "%.3f", 0 ));
}
int ui2_float2(const char *label, float v[2]) {
//    return UI2_LABEL(label, igInputFloat2("##", v, "%f", ui2_slider_flags));
    return UI2_LABEL(label, igDragFloat2("##", v, 0.005f, 0, 0, "%.3f", 0 ));
}
int ui2_float3(const char *label, float v[3]) {
//    return UI2_LABEL(label, igInputFloat3("##", v, "%f", ui2_slider_flags));
    return UI2_LABEL(label, igDragFloat3("##", v, 0.005f, 0, 0, "%.3f", 0 ));
}
int ui2_float4(const char *label, float v[4]) {
//    return UI2_LABEL(label, igInputFloat4("##", v, "%f", ui2_slider_flags));
    return UI2_LABEL(label, igDragFloat4("##", v, 0.005f, 0, 0, "%.3f", 0 ));
}
int ui2_slider(const char *label, float *value) {
    return UI2_LABEL(label, igSliderFloat("##", value, 0, 1, "%.2f", ui2_slider_flags));
}
int ui2_slider2(const char *label, float *pct, const char *caption) {
    if( !UI2_LABEL_1OF2(label) )
        return 0;

    igGetIO()->ConfigWindowsMoveFromTitleBarOnly = true;
//    igPushStyleColor_Vec4(ImGuiCol_PlotHistogram, *igGetStyleColorVec4(ImGuiCol_PlotLinesHovered));

    ImVec2 area;
    igGetContentRegionAvail(&area);
    unsigned w = area.x; // avail width
    unsigned h = igTextLineHeight(); // line height

    int ret = 0;
    igProgressBar(*pct,ImVec2(-1,0),caption);

    if( igIsItemHovered(0) && mouse('l') ) {
        ImVec2 click, src, dst;
        igGetMousePos(&click);
        igGetItemRectMin(&src);
        *pct = (click.x - src.x) / (area.x-2);
        if(*pct < 0) *pct = 0; else if(*pct > 1) *pct = 1;
        ret = 1;
    }

//    igPopStyleColor(1);
    //igGetIO()->ConfigWindowsMoveFromTitleBarOnly = false;

    return UI2_LABEL_2OF2(ret);
}

int ui2_mat33(const char *label, float M[9]) {
//    ui2_num_signs = 3;
    int changed = 0;
    changed |= ui2_label(label);
    changed |= ui2_float3("[0,1,2]", M);
    changed |= ui2_float3("[3,4,5]", M+3);
    changed |= ui2_float3("[6,7,8]", M+6);
    return changed;
}
int ui2_mat34(const char *label, float M[12]) {
//    ui2_num_signs = 3;
    int changed = 0;
    changed |= ui2_label(label);
    changed |= ui2_float4("[0,1,2,3]", M);
    changed |= ui2_float4("[4,5,6,7]", M+4);
    changed |= ui2_float4("[8,9,A,B]", M+8);
    return changed;
}
int ui2_mat44(const char *label, float M[16]) {
//    ui2_num_signs = 4;
    int changed = 0;
    changed |= ui2_label(label);
    changed |= ui2_float4("[0,1,2,3]", M);
    changed |= ui2_float4("[4,5,6,7]", M+4);
    changed |= ui2_float4("[8,9,A,B]", M+8);
    changed |= ui2_float4("[C,D,E,F]", M+12);
    return changed;
}


int ui2_color3f(const char *label, float color[3]) { //[0..1]
    // |ImGuiColorEditFlags_PickerHueWheel
    return UI2_LABEL(label, igColorEdit3("##", color, ImGuiColorEditFlags_Float));
}
int ui2_color3(const char *label, unsigned *color32) { //[0..255]
    ImVec4 color; igColorConvertU32ToFloat4(&color,(ImU32)*color32);
    int touched = ui2_color3f(label, &color.x);
    if( touched ) *color32 = (unsigned)igColorConvertFloat4ToU32(color);
    return touched;
}

int ui2_color4f(const char *label, float color[4]) { //[0..1]
    // |ImGuiColorEditFlags_PickerHueWheel
    return UI2_LABEL(label, igColorEdit4("##", color, ImGuiColorEditFlags_Float|ImGuiColorEditFlags_AlphaPreviewHalf|ImGuiColorEditFlags_AlphaBar));
}
int ui2_color4(const char *label, unsigned *color32) { //[0..255]
    ImVec4 color; igColorConvertU32ToFloat4(&color,(ImU32)*color32);
    int touched = ui2_color4f(label, &color.x);
    if( touched ) *color32 = (unsigned)igColorConvertFloat4ToU32(color);
    return touched;
}

int ui2_button(const char *label) {
    igPushID_Int(++ui2_global_id);
    int rc = igButton2(label, ImVec2(-1,0));
    return igPopID(), rc;
}
int ui2_buttons(int buttons, /*labels*/...) {
    const char *labels[16] = {0};
    if( buttons >= COUNTOF(labels) ) die("ui2_buttons() capacity exceeded");
        va_list list;
        va_start(list, buttons);
        for( int i = 0; i < buttons; ++i ) {
            labels[i] = va_arg(list, const char*);
        }
        va_end(list);

    int touched = 0;

    igPushID_Int(++ui2_global_id);

    // special case
    if( buttons == 2 && !strcmp(labels[1], "...") ) {
        igSetNextItemAllowOverlap();
        if( igButton2(labels[0], ImVec2(-1,0)) ) touched = 1;
        igSameLine(0,0);
        igSetCursorPosX(igGetCursorPosX()-20);
        if( igSmallButton(labels[1]) ) touched = 2;
    }

    // general case
    else {
        igColumns(buttons, "##", false);
        for( int i = 0; i < buttons; ++i ) {
            if( igButton2(labels[i], ImVec2(-1,0)) )
                touched = i + 1;
            igNextColumn();
        }
        igColumns(1, "", false);
    }

    igPopID();

    return touched;
}
int ui2_toolbar(const char *label, int icons, ...) {
    if( !UI2_LABEL_1OF2(label) )
        return 0;

    const char *labels[16] = {0};
    if( icons >= COUNTOF(labels) ) die("ui2_toolbar() capacity exceeded");
        va_list list;
        va_start(list, icons);
        for( int i = 0; i < icons; ++i ) {
            labels[i] = va_arg(list, const char*);
        }
        va_end(list);

    igPushStyleVar_Float(ImGuiStyleVar_FrameRounding, 0);

    int icon = 0;
    for( int i = 0; i < icons ; ++i ) {
        if( /*igSmallButton*/igButton2(labels[i],ImVec2(0,0))) {
            icon = i+1;
        }
        if( i < (icons-1) ) igSameLine(0,0);
    }

    igPopStyleVar(1);

    return UI2_LABEL_2OF2(icon);
}
int ui2_radio(const char *label, int *selected, int icons, ...) {
    if( !UI2_LABEL_1OF2(label) )
        return 0;

    const char *labels[16] = {0};
    if( icons >= COUNTOF(labels) ) die("ui2_radio() capacity exceeded");
        va_list list;
        va_start(list, icons);
        for( int i = 0; i < icons; ++i ) {
            labels[i] = va_arg(list, const char*);
        }
        va_end(list);

    igPushStyleVar_Float(ImGuiStyleVar_FrameRounding, 0);

    int icon = 0;
    for( int i = 0; i < icons ; ++i ) {
        int highlighted = i == *selected;
        if( highlighted ) igPushStyleColor_Vec4(ImGuiCol_Button, *igGetStyleColorVec4(ImGuiCol_ButtonActive));

        if( /*igSmallButton*/igButton2(labels[i],ImVec2(0,0))) {
            icon = i+1;
            *selected = i;
        }

        if( highlighted ) igPopStyleColor(1);

        if( i < (icons-1) ) igSameLine(0,0);
    }

    igPopStyleVar(1);

    return UI2_LABEL_2OF2(icon);
}

// @fixme: show arg not needed?
int ui2_dialog(const char *label, const char *text, int choices) {// @fixme: return
    int selected = 0;

    if (igButton2(label,ImVec2(-1,0)))
        igOpenPopup_Str(label,0);

    if (igBeginPopupModal(label, NULL, 0/*ImGuiWindowFlags_MenuBar*/)) {
        igText("%s", text);

        if( choices > 0 ) {
            selected = ui2_buttons(choices > 3 ? 3 : choices, "OK", "Cancel", "Retry");
            if( selected ) {
                igCloseCurrentPopup();
            }
        } else {
            if (igButton2("Close",ImVec2(0,0)))
                selected = 1, igCloseCurrentPopup();
        }

        igEndPopup();
    }
    return selected;
}
int ui2_alert(const char *message, int *open) {
    if(*open == 1) igOpenPopup_Str("Alert", 0), *open = 2;

    if (igBeginPopupModal("Alert", NULL, 0/*ImGuiWindowFlags_MenuBar*/)) {
        igText("%s", message);
        int selected = ui2_buttons(1, "OK");
        if( selected ) igCloseCurrentPopup(), *open = 0;
        igEndPopup();
    }
    return *open;
}
int ui2_bitmask(const char *label, int numbits, void *ptr) {
    if( numbits ==  8 ) label = va("%s (%02x)", label, *(uint8_t*)ptr);
    if( numbits == 16 ) label = va("%s (%04x)", label, *(uint16_t*)ptr);
    if( numbits == 32 ) label = va("%s (%08x)", label, *(uint32_t*)ptr);
    if( numbits == 64 ) label = va("%s (%016llx)", label, *(uint64_t*)ptr);

    if( !UI2_LABEL_1OF2(label) )
        return 0;

    float *FramePaddingX = &igGetStyle()->FramePadding.x;
    float FramePaddingX_Restore = *FramePaddingX;
    if( numbits > 8 ) *FramePaddingX = 0;

    uintptr_t addr = 0;
    memcpy(&addr, ptr, numbits/8);

    int touched = 0;
    for( int i = 0; i < numbits ; ++i ) {
        int j = ifdef(V3_BIG,numbits - 1 - i,i);
        if( i > 0 ) igSameLine(0,0);
        igPushID_Int(i);
        if( igSmallButton(addr & 1ull<<j ? "1" : "0") ) {
            touched = i+1;
            addr ^= 1ull<<j;
            memcpy(ptr, &addr, numbits/8);
        }
        if( igIsItemHovered(0) )
            igSetTooltip(va("Bit %d", j));
        igPopID();
    }

    *FramePaddingX = FramePaddingX_Restore;

    return UI2_LABEL_2OF2(touched);
}

int ui2_list(const char *label, int *selector, int num_items, const char **items) {
    igPushStyleColor_Vec4(ImGuiCol_Header, *igGetStyleColorVec4(ImGuiCol_ButtonActive));
    int height_in_items = num_items > 4 ? 4 : num_items;
    int rc = UI2_LABEL(label, igListBox_Str_arr("##",selector,items,num_items,height_in_items));
    igPopStyleColor(1);
    return rc;
}

int ui2_separator(const char *label) {
    ui2_hue = (ui2_hue + 1) % 7;
    ImVec4 color; ui2_hue32 = igGetColorU32_Vec4((ImColor_HSV((ImColor*)&color, ui2_hue / 7.0f, 0.7f, 0.7f, 1), color));

    if( label && ui2_filter && ui2_filter[0] ) if( !strstri(label, ui2_filter) ) return 0;

        if(ui2_fonts[1]) igPushFont(ui2_fonts[1], 0);

#if 0 // colorful
        igPushStyleColor_U32(ImGuiCol_Separator, ui2_hue32);
        int ret = UI2_LABEL(NULL, (igSeparatorText(label ? label : ""), 0));
        igPopStyleColor(1);
#else
        int ret = (igSeparatorText(label ? label : ""), 0);
#endif

        if(ui2_fonts[1]) igPopFont();

    return ret;
}

int ui2_section(const char *label) {
    ui2_hue = (ui2_hue + 1) % 7;
    ImVec4 color; ui2_hue32 = igGetColorU32_Vec4((ImColor_HSV((ImColor*)&color, ui2_hue / 7.0f, 0.7f, 0.7f, 1), color));

    igPushID_Int(++ui2_global_id);
    int ImGuiTreeNodeFlags_flags = 0;
    int ret = igCollapsingHeader_TreeNodeFlags(label,ImGuiTreeNodeFlags_flags);
    igPopID();
    return ret;
}

int ui2_subimage(const char *label, unsigned id, unsigned w, unsigned h, float u0, float v0, float u1, float v1) {
    if( !UI2_LABEL_1OF2(label) )
        return 0;

    ImVec2 area;
    igGetContentRegionAvail(&area);
    if( w == ~0u ) w = area.x; // avail width
    if( h == ~0u ) h = igTextLineHeight(); // line height

    ImTextureRef* ref = ImTextureRef_ImTextureRef_TextureID(id);

    int ret = igImageButtonEx(++ui2_global_id, *ref,
        ImVec2(w,h), ImVec2(u0,v0), ImVec2(u1,v1), ImVec4(0,0,0,0), ImVec4(1,1,1,1), 0);

    ImTextureRef_destroy(ref);

    if( ret ) {
        ImVec2 click, src, dst;
        igGetMousePos(&click);
        igGetItemRectMin(&src);
        int px = 0x100 * (click.x - src.x) / area.x;
        int py = 0x100 * (click.y - src.y) / area.y;
        ret = px * 0x100 + py; // printf("%d %d xy:%d\n", px, py, ret);
    }

    return UI2_LABEL_2OF2(ret);
}

int ui2_image(const char *label, unsigned id, unsigned w, unsigned h) {
    return ui2_subimage(label, id, w,h, 0,0,1,1);
}

int ui2_is_enabled = 1;
int ui2_enable(bool enable) {
    if( enable ^ ui2_is_enabled ) {
        if( enable ) igEndDisabled(); else igBeginDisabled(1);
    }
    ui2_is_enabled = enable;
    return 1;
}
int ui2_enabled() {
    return ui2_is_enabled;
}


int ui2_is_hidden = 0;
int ui2_hide(bool hidden) {
    ui2_is_hidden = hidden;
    return 1;
}
int ui2_hidden() {
    return ui2_is_hidden;
}


int ui2_hovered() {
    return igIsAnyItemHovered() || igIsAnyWindowHovered();
}
int ui2_active() {
    return ui2_hovered() || igIsAnyItemActive();
}


int ui2_demo(int do_windows) {
    static int integer = 42;
    static bool boolean = true;
    static float floating = 3.14159;
    static float float2[2] = {1,2};
    static float float3[3] = {1,2,3};
    static float float4[4] = {1,2,3,4};
    static float rgbf[3] = {0.84,0.67,0.17};
    static float rgbaf[4] = {0.67,0.90,0.12,0.5};
    static unsigned rgb = 0x22446688;
    static unsigned rgba = 0x88664422;
    static float slider = 0.5f;
    static float slider2 = 0.5f;
    static char buffer[64] = "hello world 123";
    static char *string;
    static char *text;
    static int item; const char *list[] = {"one","two","three"};
    static uint8_t bitmask = 0x55;
    static int hits;
    static int window1, window2, window3;
    static int disable_all;
    static int radio;

    if( ui2_panel("UI", UI2_MENUS) ) {
        // display tray, and menu section
        if( ui2_tray() ) {
            if( ui2_menu("File") ) {
                if (ui2_case("Some menu item", 0)) {}
                ui2_menu_end();
            }
            ui2_tray_end();
        }

        if( ui2_separator("Labels")) {}
        if( ui2_label("my label")) {}
        if( ui2_label("my label with tooltip@built on " __DATE__ " " __TIME__)) {}
        if( ui2_link("my link", "https://google.com"));

        if( ui2_separator("Types")) {}
        if( ui2_bool("my bool", &boolean) ) puts("bool changed");
        if( ui2_int("my int", &integer) ) puts("int changed");
        if( ui2_float("my float", &floating) ) puts("float changed");

        if( ui2_separator("Strings") ) {}
        if( ui2_buffer("my buffer", buffer, countof(buffer)) ) puts("buffer changed");
        if( ui2_string("my string", &string) ) puts("string changed");
        if( ui2_text("my text", 3, &text) ) puts("text changed");

        if( ui2_separator("Lists")) {}
        if( ui2_toolbar("my toolbar", 4, UI2_ICON(STAR),UI2_ICON(STAR_HALF),UI2_ICON(BOOKMARK),UI2_ICON(BOOKMARK_STAR)) ) puts("toolbar changed");
        if( ui2_radio("my radio",&radio,3,"Radio1","Radio2","Radio3")) puts("radio changed");
        if( ui2_list("my list", &item, countof(list), list ) ) puts("list changed");

        if( ui2_separator("Sliders")) {}
        if( ui2_slider("my slider", &slider)) puts("slider changed");
        if( ui2_slider2("my slider 2", &slider2, va("%.2f", slider2))) puts("slider2 changed");
        if( ui2_percent("my bar %", slider2));
        if( ui2_percent("my bar INF", NAN));

        if( do_windows ) {
        if( ui2_separator("Windows")) {}
        int show = ui2_buttons(3, "Container", "SubPanel", "SubRender");
        if( show == 1 ) window1 = 1;
        if( show == 2 ) window2 = 1;
        if( show == 3 ) window3 = 1;
        }

        if( ui2_separator("Vectors") ) {}
        if( ui2_float2("my float2", float2) ) puts("float2 changed");
        if( ui2_float3("my float3", float3) ) puts("float3 changed");
        if( ui2_float4("my float4", float4) ) puts("float4 changed");

        if( ui2_separator("Colors")) {}
        if( ui2_color3("my color3", &rgb) ) puts("color3 changed");
        if( ui2_color4("my color4@this is a tooltip", &rgba) ) puts("color4 changed");
        if( ui2_color3f("my color3f", rgbf) ) puts("color3f changed");
        if( ui2_color4f("my color4f@this is a tooltip", rgbaf) ) puts("color4f changed");

        if( ui2_separator("Others")) {}
        if( ui2_bitmask("my bitmask", 8, &bitmask) ) printf("bitmask changed %x\n", bitmask);

        int xy;
        if( (xy = ui2_image("my image", 1/*image id #1*/, -1, -1)) ) { printf("image clicked at %04x\n", xy); }

        if( ui2_separator("Buttons")) {}

        static int show_alert = 0;
        show_alert = ui2_dialog("my dialog", __FILE__ "\n" __DATE__ "\n" "Public Domain.\nPress YES to display an alert.", 2/*two buttons*/);
        ui2_alert("Button was pressed", &show_alert);

        if( ui2_button("my toggle") ) { disable_all ^= 1; }
        if( disable_all ) ui2_enable(0);

        if( ui2_button("my toast") )
            ui2_notify(va("My random toast (%d)", rand()), va("This is notification #%d", ++hits));

        if( ui2_buttons(2, "yes", "no") ) puts("button2 clicked");
        if( ui2_buttons(3, "yes", "no", "maybe") ) puts("button3 clicked");

        if( disable_all ) ui2_enable(1); // restore enabled state
        ui2_panel_end();
    }

    if( !do_windows ) return 0;


    // window api showcasing
    if( ui2_window("Container demo", &window1) ) {
        ui2_label("label #1");
        if( ui2_bool("my bool", &boolean) ) puts("bool changed");
        if( ui2_int("my int", &integer) ) puts("int changed");
        if( ui2_float("my float", &floating) ) puts("float changed");
        if( ui2_buffer("my string", string, 64) ) puts("string changed");
        ui2_window_end();
    }

    if( ui2_window("SubPanel demo", &window2) ) {
        if( ui2_panel("panel #2", 0) ) {
            ui2_label("label #2");
            if( ui2_bool("my bool", &boolean) ) puts("bool changed");
            if( ui2_int("my int", &integer) ) puts("int changed");
            if( ui2_float("my float", &floating) ) puts("float changed");
            if( ui2_buffer("my string", string, 64) ) puts("string changed");
            ui2_panel_end();
        }
        ui2_window_end();
    }

    if( ui2_window("SubRender demo", &window3) ) {
        if( ui2_panel("panel #3A", 0) ) {
            if( ui2_bool("my bool", &boolean) ) puts("bool changed");
            if( ui2_int("my int", &integer) ) puts("int changed");
            if( ui2_float("my float", &floating) ) puts("float changed");
            if( ui2_buffer("my string", string, 64) ) puts("string changed");
            if( ui2_separator(NULL) ) {}
            if( ui2_slider("my slider", &slider)) puts("slider changed");
            if( ui2_slider2("my slider 2", &slider2, va("%.2f", slider2))) puts("slider2 changed");
            ui2_panel_end();
        }
        if( ui2_panel("panel #3B", 0) ) {
            if( ui2_bool("my bool", &boolean) ) puts("bool changed");
            if( ui2_int("my int", &integer) ) puts("int changed");
            if( ui2_float("my float", &floating) ) puts("float changed");
            if( ui2_buffer("my string", string, 64) ) puts("string changed");
            if( ui2_separator(NULL) ) {}
            if( ui2_slider("my slider", &slider)) puts("slider changed");
            if( ui2_slider2("my slider 2", &slider2, va("%.2f", slider2))) puts("slider2 changed");
            ui2_panel_end();
        }

#if HAS_IMAGE
        const char *title = "SubRender demo";
        struct nk_window *win = nk_window_find(ui2_ctx, title);
        if( win ) {
            enum { menubar_height = 65 }; // title bar (~32) + menu bounds (~25)
            struct nk_rect bounds = win->bounds; bounds.y += menubar_height; bounds.h -= menubar_height;
#if 1
            ddraw_flush();

            // @fixme: this is breaking rendering when post-fxs are in use. edit: cannot reproduce
            static texture_t tx = {0};
            if( texture_rec_begin(&tx, bounds.w, bounds.h )) {
                glClearColor(0.15,0.15,0.15,1);
                glClear(GL_COLOR_BUFFER_BIT);
                ddraw_grid(10);
                ddraw_flush();
                texture_rec_end(&tx);
            }
            struct nk_image image = nk_image_id((int)tx.id);
            nk_draw_image_flipped(nk_window_get_canvas(ui2_ctx), bounds, &image, nk_white);
#else
            static video_t *v = NULL;
            do_once v = video( "bjork-all-is-full-of-love.mp4", VIDEO_RGB );

            texture_t *textures = video_decode( v );

            struct nk_image image = nk_image_id((int)textures[0].id);
            nk_draw_image(nk_window_get_canvas(ui2_ctx), bounds, &image, nk_white);
#endif
        }
#endif

        ui2_window_end();
    }
    return 0;
}

#endif

#if 0
// How to do a toolbar in Dear ImGui.
// [src] https://gist.github.com/rmitton/f80cbb028fca4495ab1859a155db4cd8

enum { toolbarSize = 50 }; // cant be probably smaller than padding amounts added up together
float menuBarHeight;

void DockSpaceUI() {
    ImGuiViewport* viewport = igGetMainViewport();
    igSetNextWindowPos(ImVec2(viewport->Pos.x,viewport->Pos.y + toolbarSize), 0, ImVec2(0,0));
    igSetNextWindowSize(ImVec2(viewport->Size.x,viewport->Size.y - toolbarSize), 0);
    igSetNextWindowViewport(viewport->ID);
    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    igPushStyleVar_Vec2(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    igPushStyleVar_Float(ImGuiStyleVar_WindowRounding, 0.0f);
    igPushStyleVar_Float(ImGuiStyleVar_WindowBorderSize, 0.0f);
    igBegin("Master DockSpace", NULL, window_flags);
    ImGuiID dockMain = igGetID_Str("MyDockspace");
    
    // Save off menu bar height for later.
    menuBarHeight = igGetCurrentWindow()->MenuBarHeight;

    igDockSpace(dockMain, ImVec2(0,0), 0, NULL);
    igEnd();
    igPopStyleVar(3);
}

void ToolbarUI() {
    ImGuiViewport* viewport = igGetMainViewport();
    igSetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + menuBarHeight), 0, ImVec2(0,0));
    igSetNextWindowSize(ImVec2(viewport->Size.x, toolbarSize), 0);
    igSetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = 0
        | ImGuiWindowFlags_NoDocking 
        | ImGuiWindowFlags_NoTitleBar 
        | ImGuiWindowFlags_NoResize 
        | ImGuiWindowFlags_NoMove 
        | ImGuiWindowFlags_NoScrollbar 
        | ImGuiWindowFlags_NoSavedSettings
        ;
    igPushStyleVar_Float(ImGuiStyleVar_WindowBorderSize, 0);
    igBegin("TOOLBAR", NULL, window_flags);
  
    igButton2("Toolbar goes here", ImVec2(0,37));
  
    igEnd();

    igPopStyleVar(1);
}

void ProgramUI() {
    DockSpaceUI();
    ToolbarUI();
}
#endif

#if 0

API int   ui2_contextual();
API int    ui2_short(const char *label, short *value);
API int    ui2_double(const char *label, double *value);
API int   ui2_contextual_end(int close);

// @todo: move to editor
API int  ui2_display(const char *panel_or_window_title, int enabled);
API int  ui2_setsize(const char *panel_or_window_title, float width, float height);
API int ui2_popups(); // returns number of active popups

#endif

#if CODE

int ui2_loadfont(unsigned slot, float pt, float brightness, const char *resource, const void *runes) {
    ImFontConfig*cfg = ImFontConfig_ImFontConfig();

    bool ok = 0;
    if( slot < countof(ui2_fonts) && pt && brightness && resource ) {
        cfg->FontDataOwnedByAtlas = false; // TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).
        cfg->RasterizerMultiply = brightness;
        int len; char *bin = file_read(resource, &len);
        ok = !!(ui2_fonts[slot] = ImFontAtlas_AddFontFromMemoryTTF(igGetIO()->Fonts, bin, len, pt, cfg, (const ImWchar *)runes));
    } else {
        ok = !!(ui2_fonts[slot] = ImFontAtlas_AddFontDefault(igGetIO()->Fonts, cfg));
    }

    ImFontConfig_destroy(cfg);
    return ok;
}

int ui2_loadfonts() { // return num loaded slots

#if 1
    #define UI2_FONT0 16.f, 1.00f, "B612Regular.ttf"
    #define UI2_FONT1 16.f, 1.50f, "B612BoldItalic.ttf"
#endif

    // load primary font [0]
    // else load os fallbacks for primary font [0]
    // else load embedded default font
    if( !ui2_loadfont(0, UI2_FONT0, NULL) )
#if !V3_MACOS
        if( !ui2_loadfont(0, 15.f, 1.f, ifdef(V3_WINDOWS,"C:/Windows/Fonts/Arial.ttf","/usr/share/fonts/liberation/LiberationSans-Regular.ttf"),NULL) )
#endif
            if( !ui2_loadfont(0, 0, 0, 0, NULL)) ;
    
    // merge icon glyphs in primary font
    // [x] MaterialSymbolsOutlined_28pt-Regular.ttf
    // [x] MaterialSymbolsRounded_28pt-Regular.ttf
    // [x] MaterialSymbolsSharp_Filled_28pt-Regular.ttf
    int len; char *bin = file_read("MaterialSymbolsRounded_28pt-Regular.ttf", &len);
    if( bin && len ) {
        static const unsigned/*ImWchar*/ runes[] = { ICON_MIN_MS, ICON_MAX_MS, 0, };

        ImFontConfig*cfg = ImFontConfig_ImFontConfig();
            cfg->FontDataOwnedByAtlas = false; // TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).
            cfg->OversampleH = 1; // Rasterize at higher quality for sub-pixel positioning. Note the difference between 2 and 3 is minimal. You can reduce this to 1 for large glyphs save memory. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
            cfg->OversampleV = 1; // Rasterize at higher quality for sub-pixel positioning. This is not really useful as we don't use sub-pixel positions on the Y axis.
            cfg->PixelSnapH = true;    // Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
            // ImVec2          GlyphExtraSpacing;      // 0, 0     // Extra spacing (in pixels) between glyphs. Only X axis is supported for now.
            cfg->GlyphOffset.y = 5;  // MD:8           // 0, 0     // Offset all glyphs from this font input.
            // const ImWchar*  GlyphRanges;            // NULL     // THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE. Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list).
            // float           GlyphMinAdvanceX;       // 0        // Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font
            // float           GlyphMaxAdvanceX;       // FLT_MAX  // Maximum AdvanceX for glyphs
            cfg->MergeMode = true; // Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
            // unsigned int    FontBuilderFlags;       // 0        // Settings for custom font builder. THIS IS BUILDER IMPLEMENTATION DEPENDENT. Leave as zero if unsure.
            // float           RasterizerMultiply;     // 1.0f     // Linearly brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable. This is a silly thing we may remove in the future.
            // float           RasterizerDensity;      // 1.0f     // DPI scale for rasterization, not altering other font metrics: make it easy to swap between e.g. a 100% and a 400% fonts for a zooming display. IMPORTANT: If you increase this it is expected that you increase font scale accordingly, otherwise quality may look lowered.
            // ImWchar         EllipsisChar;           // -1       // Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.
            ImFontAtlas_AddFontFromMemoryTTF(igGetIO()->Fonts, bin, len, 23.5f, cfg, (const ImWchar*)runes);
        ImFontConfig_destroy(cfg);
    }

    // load bold font [1]
    // else load embedded default font
    if( !ui2_loadfont(1, UI2_FONT1, NULL) )
        ui2_fonts[1] = ui2_fonts[0];

    return !!ui2_fonts[0] + !!ui2_fonts[1] + !!ui2_fonts[2] + !!ui2_fonts[3];
}

int ui2_ivec2(const char *label, ivec2 *v) { return ui2_unsigned2(label, (unsigned*)v); }
int ui2_ivec3(const char *label, ivec3 *v) { return ui2_unsigned3(label, (unsigned*)v); }
int ui2_vec2(const char *label, vec2 *v) { return ui2_float2(label, (float*)v); }
int ui2_vec3(const char *label, vec3 *v) { return ui2_float3(label, (float*)v); }
int ui2_vec4(const char *label, vec4 *v) { return ui2_float4(label, (float*)v); }

int ui2_link(const char *label, const char *url) {
    return UI2_LABEL(label, (igTextLinkOpenURL(url, url), 0));
}

int ui2_collapse(const char *label, const char *id) {
    if(!UI2_LABEL_1OF2(""))
        return 0;

    igIndent(12);
    int ret = igTreeNodeEx_StrStr(id,0|ImGuiTreeNodeFlags_CollapsingHeader,"%s",label);
    igIndent(-12);
    if(ret) igIndent(4);

    return UI2_LABEL_2OF2(ret);
}
int ui2_collapse_end() {
    return igIndent(-4), 0;
}
//#define ui2_collapse_clicked() 0



#endif
