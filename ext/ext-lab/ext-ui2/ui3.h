#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

#define igGetIO igGetIO_Nil

#define igInsertNotification(...) 0

int igThemeV3(int hue07, int alt07, int nav07, int light01, int compact01, int border01, int shape03);
int igThemeV3_(const char *magic);

#define IMGUI_THEME        igThemeV3(6,4,0,0,1,1,1), ui2_loadfonts();
#define IMGUI_SETTINGS     io.IniFilename = option("--ui.ini", ".settings.ini");
#define IMGUI_DEPS_A       igFileDialogInit(), ImPlot_CreateContext(), ImPlot_SetImGuiContext(ctx);
#define IMGUI_DEPS_B       /* Notifications: style setup + background color */ \
                           ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f); /* Disable round borders */ \
                           ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f); /* Disable borders */ \
                               ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f));  \
                                   igRenderNotifications(); \
                               ImGui::PopStyleColor(1); \
                           ImGui::PopStyleVar(2);


typedef struct ivec2 { int x, y; } ivec2;
typedef struct ivec3 { int x, y, z; } ivec3;
typedef struct ivec4 { int x, y, z, w; } ivec4;

#define UI2_ICON(GLYPH) ICON_MS_##GLYPH // @fixme: ui2_iconsloaded() depending on whether icon font was loaded
#if 1
#include "ext-ui2/3rd_icon_ms.h"
#else
enum {
        ICON_MIN_MS,
        ICON_MAX_MS,
};
#define ICON_MS_INFO "INFO"
#define ICON_MS_STAR "STAR"
#define ICON_MS_STAR_HALF "STAR_HALF"
#define ICON_MS_BOOKMARK "BOOKMARK"
#define ICON_MS_BOOKMARK_STAR "BOOKMARK_STAR"
#define ICON_MS_SEARCH "SEARCH"
#endif

#define REALLOC realloc
#define countof COUNTOF
#define strcatf(p,...) (*p ? *p : (*p = calloc(1,1)))

#define ImVec2(x,y) ((ImVec2){x,y})
#define ImVec4(x,y,z,w) ((ImVec4){x,y,z,w})

#define file_read(url, plen) (*plen = file_size(lookup(url)), file_read(lookup(url))).str


#undef CODE
#include "game_ui2.h"
#define CODE 1
#include "game_ui2.h"


void dimgui_begin(void) {
    ONCE {
        // setup imgui
        igCreateContext(NULL);

        // set docking
        ImGuiIO *ioptr = igGetIO();
        ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        //ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    #ifdef IMGUI_HAS_DOCK
        ioptr->ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
        //ioptr->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows
    #endif

        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only

        // Setup scaling
        ImGuiStyle* style = igGetStyle();
        ImGuiStyle_ScaleAllSizes(style, main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style->FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
        ioptr->ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
        ioptr->ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
#endif
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        //ImGui_ImplGlfw_SetCallbacksChainForAllWindows(true);

        const char *glsl_version = ifdef(V3_MACOS, "#version 150", "#version 130");
        ImGui_ImplOpenGL3_Init(glsl_version);

        igStyleColorsDark(NULL);
        // ImFontAtlas_AddFontDefault(io.Fonts, NULL);

        IMGUI_THEME
    }

    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();
}
void dimgui_end(void) {
    // render
    igRender();
    glfwMakeContextCurrent(window);
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
    ImGuiIO *ioptr = igGetIO();
    if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
    {
        GLFWwindow *backup_current_window = glfwGetCurrentContext();
        igUpdatePlatformWindows();
        igRenderPlatformWindowsDefault(NULL, NULL);
        glfwMakeContextCurrent(backup_current_window);
    }
#endif
}
void dimgui_drop() {
    // clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);
}
