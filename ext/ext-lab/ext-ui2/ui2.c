#define V3_IMPLEMENTATION
#include "v3.h"

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

#define igGetIO igGetIO_Nil

#define igInsertNotification(...) 0

int igThemeV3(char hue, char alt, char nav, bool light, bool compact, bool rounded, bool borders);

#define IMGUI_THEME        igThemeV3(0,0,3,0,0,0,0), ui2_loadfonts();
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











int main(int argc, char *argv[])
{
    extern GLFWwindow *window;

    if (!glfwInit())
        return -1;

    // Decide GL+GLSL versions
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char *glsl_version = "#version 150";
#else
    // GL 3.2 + GLSL 130
    const char *glsl_version = "#version 130";
#endif

    // just an extra window hint for resize
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "v3(ui3)", NULL, NULL);

    if (!window) 
    {
        printf("Failed to create window! Terminating!\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    event(on_graphics_context_created);

    // enable vsync
    glfwSwapInterval(1);

    // check opengl version sdl uses
    printf("opengl version: %s\n", (char *)glGetString(GL_VERSION));

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


        // Setup scaling
        ImGuiStyle* style = igGetStyle();
        ImGuiStyle_ScaleAllSizes(style, main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style->FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
        ioptr->ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
        ioptr->ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
#endif
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    igStyleColorsDark(NULL);
    // ImFontAtlas_AddFontDefault(io.Fonts, NULL);

IMGUI_THEME

    bool showDemoWindow = true;
    bool showAnotherWindow = false;
    ImVec4 clearColor;
    clearColor.x = 0.00f;
    clearColor.y = 0.00f;
    clearColor.z = 0.08f;
    clearColor.w = 1.00f;

    // main event loop
    bool quit = false;
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        // start imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        igNewFrame();

ui2_tick();

        if (showDemoWindow)
            igShowDemoWindow(&showDemoWindow);

        // show a simple window that we created ourselves.
        {
            static int counter = 0;

            igBegin("Hello, world!", NULL, 0);
            igText("This is some useful text");
            igCheckbox("Demo window", &showDemoWindow);
            igCheckbox("Another window", &showAnotherWindow);

            int theme = 0;
            static int hue = 0, alt = 0, nav = 0;
            static bool light = 0, compact = 0, rounded = 0, borders = 0;
            theme |= igSliderInt("Hue", &hue, 0, 7, 0, 0);
            theme |= igSliderInt("Alt", &alt, 0, 7, 0, 0);
            theme |= igSliderInt("Nav", &nav, 0, 7, 0, 0);
            theme |= igCheckbox("Light", &light);
            theme |= igCheckbox("Compact", &compact);
            theme |= igCheckbox("Rounded", &rounded);
            theme |= igCheckbox("Borders", &borders);
            if(theme) igThemeV3(hue["CRYOLAMP"], alt["CRYOLAMP"], nav["CRYOLAMP"], light, compact, rounded, borders);

            igColorEdit3("clear color", (float *)&clearColor, 0);

            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Button", buttonSize))
                counter++;
            igSameLine(0.0f, -1.0f);
            igText("counter = %d", counter);

            igText("Application average %.3f ms/frame (%.1f FPS)",
                         1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
            igEnd();
        }

        if (showAnotherWindow) 
        {
            igBegin("imgui Another Window", &showAnotherWindow, 0);
            igText("Hello from imgui");
            ImVec2 buttonSize;
            buttonSize.x = 0;
            buttonSize.y = 0;
            if (igButton("Close me", buttonSize)) {
                showAnotherWindow = false;
            }
            igEnd();
        }


ui2_demo(1);



        // render
        igRender();
        glfwMakeContextCurrent(window);
        glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
#ifdef IMGUI_HAS_DOCK
        if (ioptr->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
        {
            GLFWwindow *backup_current_window = glfwGetCurrentContext();
            igUpdatePlatformWindows();
            igRenderPlatformWindowsDefault(NULL, NULL);
            glfwMakeContextCurrent(backup_current_window);
        }
#endif
        glfwSwapBuffers(window);
    }

    // clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
