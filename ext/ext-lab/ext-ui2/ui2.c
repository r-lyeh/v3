#define V3_IMPLEMENTATION
#include "v3.h"

#include "ui3.h"

int main(int argc, char *argv[])
{
    extern GLFWwindow *window;

#if 0
    if (!glfwInit())
        return -1;

    // Decide GL+GLSL versions: apple( 3.2 core + glsl150 ), others( 3.2 core + glsl130 )
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // just an extra window hint for resize
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), "v3(ui3)", NULL, NULL);
    if (!window) die("Failed to create window! Terminating!\n");

    glfwMakeContextCurrent(window);

    event(on_graphics_context_created);

    // enable vsync
    glfwSwapInterval(1);

    // check opengl version sdl uses
    printf("opengl version: %s\n", (char *)glGetString(GL_VERSION));
#else
    app_create(.title = "v3(ui3)", .extra = "gl32");
#endif

    bool showDemoWindow = true;
    bool showAnotherWindow = false;
    ImVec4 clearColor = {0,0,0.08,1};

    // main event loop
    while (app_swap(.color = color24(clearColor.x*255,clearColor.y*255,clearColor.z*255)))
    {
        // imgui needs all end-user's glfw callbacks pre-configured at init time.
        // we call app_swap() once to simulate the on_frame_begin/on_frame_end events that will config all glfw callbacks.
        ONCE app_swap();
        dimgui_begin();

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

            int theme = 0; ONCE theme = 1;
            static int hue = 6, alt = 4, nav = 0;
            static bool light = 0, compact = 1, rounded = 1, borders = 1;
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

#if 0
        ImGuiIO *ioptr = igGetIO();
        glViewport(0, 0, (int)ioptr->DisplaySize.x, (int)ioptr->DisplaySize.y);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
#endif

        dimgui_end();
    }

    dimgui_drop();

    //glfwDestroyWindow(window);
    //glfwTerminate();

    return 0;
}
