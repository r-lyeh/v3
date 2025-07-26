#define V3_IMPLEMENTATION
#include "../../../v3.h"

#include "ui3.h"

#include "glfw3findmonitor.h"
#include "glfw3fullscreen.h"
#include "glfw3undecorated.h"

// Function to check if a double-click occurred
bool click2(int button, double ss) {
    if( mouse_down(button) ) {
        button = button == 'l' ? 0 : button == 'r' ? 2 : 1;

        static double down0[3] = {0};
        static double down1[3] = {0};

        down0[button] = down1[button];
        down1[button] = time_ss();

        return (down1[button] - down0[button]) < ss;
    }

    return 0;
}


typedef struct menu {
    int dummy;
    const char *icon;
    const char *label;
    const char *shortcut;
    bool enabled, selected;
    int keys;
} menu, menu_t;

#define menu(...) ((struct menu) { .label = "", .icon = "", .shortcut = NULL, .enabled = 1, .dummy = 0, ##__VA_ARGS__ })

int igMenuItem(menu x) {
    if( x.shortcut && !x.keys ) {
        const char *at1 = strchr(x.shortcut, '+');
        const char *at2 = strchr(x.shortcut, '-');
        const char *at = at1 ? at1+1 : at2 ? at2+1 : 0;
        if( at ) {
            int is_fn = ( *at == 'f' || *at <= 'F' ) && ( at[1]>= '0' && at[1]<= '9' );
            int is_nb = ( *at >= '0' && *at <= '9' );
            int is_lo = ( *at >= 'a' && *at <= 'z' );
            int is_hi = ( *at >= 'A' && *at <= 'Z' );

            /**/ if( is_fn ) x.keys = ImGuiKey_F1 - 1 + atoi(at+1);
            else if( is_nb ) x.keys = ImGuiKey_0 + atoi(at);
            else if( is_lo ) x.keys = ImGuiKey_A + *at - 'a';
            else if( is_hi ) x.keys = ImGuiKey_A + *at - 'A';

            x.keys |= strstri(x.shortcut, "ALT") ? ImGuiMod_Alt : 0;
            x.keys |= strstri(x.shortcut, "CTRL") ? ImGuiMod_Ctrl : 0;
            x.keys |= strstri(x.shortcut, "SHIFT") ? ImGuiMod_Shift : 0;
            x.keys |= strstri(x.shortcut, "SUPER") ? ImGuiMod_Super : 0;
        }
    }
    if( x.keys ) igSetNextItemShortcut(x.keys, 0|ImGuiInputFlags_Tooltip|ImGuiInputFlags_RouteGlobal);
    return igMenuItemEx(x.label, x.icon, x.shortcut, x.selected, x.enabled);
}

volatile int app_quit = 0;
#define app_quit() app_quit = 1

void app_size(int sign) {
    int state = glfwGetWindowState(window);

    if( sign < 0 ) {
        /**/ if( state == 3 ) glfwSetFullscreen(window, 0), glfwMaximizeWindow(window);
        else if( state == 2 ) glfwRestoreWindow(window);
        else if( state == 1 ) glfwIconifyWindow(window);
    } else {
        /**/ if( state == 2 ) glfwSetFullscreen(window, 1);
        else if( state == 1 ) glfwMaximizeWindow(window);
        else if( state == 0 ) glfwRestoreWindow(window);
    }
}


int main() {
    app_create(.scale = 0.85, .decorated = 0);
        extern int ui_oy; ONCE ui_oy = 25;
    while( app_swap() && !app_quit ) {

        // imgui needs all end-user's glfw callbacks pre-configured at init time.
        // we call app_swap() once to simulate the on_frame_begin/on_frame_end events that will config all glfw callbacks.
        ONCE app_swap();
        dimgui_begin();

        if( igBeginMainMenuBar() ) {

            ImGuiWindow* menubar_window = igGetCurrentWindow();
            int menubar_id = menubar_window->ID;  // Get the main menubar's unique ID

            int hovered_window_id = igGetHoveredWindowID();
            bool isHoveringTitleBar = hovered_window_id == menubar_id;
            bool isHoveringMainMenu = isHoveringTitleBar;
            bool isHoveringOther = isHoveringTitleBar || isHoveringMainMenu ? 0 : igIsAnyWindowHovered();

            #define IF_ICON(...) \
                    static int MACRO(clicked) = 0; \
                    static int MACRO(hovered) = 0; \
                    if( !MACRO(hovered) ) igPushStyleColor_Vec4(ImGuiCol_Text, *igGetStyleColorVec4(ImGuiCol_TextDisabled)); \
                    MACRO(clicked) = __VA_ARGS__; \
                    if( !MACRO(hovered) ) igPopStyleColor(1); \
                    MACRO(hovered) = igIsItemHovered(0); \
                    if( MACRO(clicked) )

            // Store the menu bar's position and size
            ImVec2 menuBarPos; igGetWindowPos(&menuBarPos);
            ImVec2 menuBarSize; igGetWindowSize(&menuBarSize);
            ImVec2 mousePos = igGetIO()->MousePos;

            static int maximized = 0;
            if( maximized > 0 ) {
                if( igBeginMenu(ICON_MS_MENU " Titlebar##x", 1) ) {
                    isHoveringMainMenu = 1;

                    menu q; ONCE q = menu( ICON_MS_CLOSE, "Quit", "Ctrl+Q" );
                    if( igMenuItem(q) ) break;

                    igEndMenu();
                }

                if( igBeginMenu("File", 1) ) {
                    isHoveringMainMenu = 1;

                    menu m; ONCE m = menu( .label = "Create" );
                    menu o; ONCE o = menu( .label = "Open", "Ctrl+O", .selected = 1 );
                    menu s; ONCE s = menu( .label = "Save", "Ctrl+S" );
                    menu q; ONCE q = menu( ICON_MS_CLOSE, "Quit", "Ctrl+Q" );

                    if( igMenuItem(m) ) {}
                    if( igMenuItem(o) ) {}
                    if( igMenuItem(s) ) {}
                    if( igMenuItem(q) ) app_quit();

                    igEndMenu();
                }

                if( igBeginMenu("Edit", 1) ) {
                    isHoveringMainMenu = 1;

                    igEndMenu();
                }
            } else {
                igPushStyleColor_Vec4(ImGuiCol_HeaderHovered, *igGetStyleColorVec4(ImGuiCol_MenuBarBg));

                IF_ICON( igBeginMenu(ICON_MS_MENU, 1) ) {
                    isHoveringMainMenu = 1;

                    maximized = 10;
                    igEndMenu();
                }

                float a = igGetStyle()->DisabledAlpha;
                igGetStyle()->DisabledAlpha = 1.f;

                if( igBeginMenu("Titlebar", 0) ) {
                    igEndMenu();
                }

                igGetStyle()->DisabledAlpha = a;

                igPopStyleColor(1);
            }

            #define IF_RIGHT_ICON(ICON) IF_ICON(igMenuItemEx((ICON), NULL, NULL, 0, 1))

                // Push the next item to the right
                ImVec2 avail; igGetContentRegionAvail(&avail);
                ImVec2 rect; igCalcTextSize(&rect, ICON_MS_MORE_VERT ICON_MS_MINIMIZE ICON_MS_STACK ICON_MS_CLOSE, 0,0,0);
                float spc = (4-1) * (igGetStyle()->ItemSpacing.x * 2) + igGetStyle()->WindowPadding.x;

                igSameLine(0,0);
                igSetCursorPosX(igGetCursorPosX() + max(0.0f, avail.x - rect.x - spc));

                igPushStyleColor_Vec4(ImGuiCol_HeaderHovered, *igGetStyleColorVec4(ImGuiCol_MenuBarBg));
                ui_label(va("%f", igGetCursorPosX()));

                    IF_RIGHT_ICON(ICON_MS_MORE_VERT) {

                    }
                    IF_RIGHT_ICON(ICON_MS_STAT_MINUS_1) { // ICON_MS_MINIMIZE
                        app_size(-1);
                    }
                    IF_RIGHT_ICON(ICON_MS_STAT_1) { // glfwGetWindowAttrib(window, GLFW_MAXIMIZED) ? ICON_MS_COLLAPSE_ALL : ICON_MS_EXPAND_ALL) {
                        app_size(1);
                    }
                    IF_RIGHT_ICON(ICON_MS_CLOSE) {
                        app_quit();
                    }

                igPopStyleColor(1);

            igEndMainMenuBar();

            if( !isHoveringMainMenu ) maximized--;

            int state = glfwGetWindowState(window);
            if( state == 1 ) { // windowed
                if( !glfwResizeUndecoratedWindow(window,~1u) ) {
                    static int dragging = 0;
                    if( isHoveringTitleBar || dragging ) dragging = glfwMoveUndecoratedWindow(window);
                    if( isHoveringTitleBar && click2('l', 0.500) ) {
                        if( state == 1 ) app_size(1), dragging = 0;
                    }
                }
            }
            if( state == 2 ) { // maximized
                if( isHoveringTitleBar && click2('l', 0.500) ) app_size(-1);
            }

            ui_label(va("%d %d %d", isHoveringTitleBar, isHoveringMainMenu, isHoveringOther));
        }

        int theme = 0; ONCE theme = 1;
        static int hue = 6, alt = 4, nav = 0, shape = 1;
        static bool light = 0, compact = 1, borders = 1;
        theme |= igCheckbox("Light", &light); igSameLine(0,0);
        theme |= igCheckbox("Compact", &compact); igSameLine(0,0);
        theme |= igCheckbox("Borders", &borders);
        theme |= igSliderInt("Shape", &shape, 0, 3, 0, 0);
        theme |= igSliderInt("Hue", &hue, 0, 7, 0, 0);
        theme |= igSliderInt("Alt", &alt, 0, 7, 0, 0);
        theme |= igSliderInt("Nav", &nav, 0, 7, 0, 0);
        if(theme)
            igThemeV3(hue["CRYOLAMP"], alt["CRYOLAMP"], nav["CRYOLAMP"], light, compact, borders, shape);

        static bool p = 1;
        igShowDemoWindow(&p);
        igShowStyleEditor(NULL);


        dimgui_end();
    }

    dimgui_drop();
}
