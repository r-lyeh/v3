#define CIMGUI_H
#include "v3.h"

#define IMGUI_IMPL_API extern "C" __declspec(dllexport)
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS 1

#include "3rd/imgui/imgui.cpp"
#include "3rd/imgui/imgui_demo.cpp"
#include "3rd/imgui/imgui_draw.cpp"
#include "3rd/imgui/imgui_tables.cpp"
#include "3rd/imgui/imgui_widgets.cpp"

#include "3rd/cimgui.cpp"
#include "3rd/cimgui_impl.cpp"

//#include "3rd/imgui/imgui_impl_allegro5.cpp"
//#include "3rd/imgui/imgui_impl_android.cpp"
//#include "3rd/imgui/imgui_impl_dx10.cpp"
//#include "3rd/imgui/imgui_impl_dx11.cpp"
//#include "3rd/imgui/imgui_impl_dx12.cpp"
//#include "3rd/imgui/imgui_impl_dx9.cpp"
#include "3rd/imgui/imgui_impl_glfw.cpp"
//#include "3rd/imgui/imgui_impl_glut.cpp"
//#include "3rd/imgui/imgui_impl_opengl2.cpp"
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "3rd/imgui/imgui_impl_opengl3.cpp"
//#include "3rd/imgui/imgui_impl_sdl2.cpp"
//#include "3rd/imgui/imgui_impl_sdl3.cpp"
//#include "3rd/imgui/imgui_impl_sdlgpu3.cpp"
//#include "3rd/imgui/imgui_impl_sdlrenderer2.cpp"
//#include "3rd/imgui/imgui_impl_sdlrenderer3.cpp"
//#include "3rd/imgui/imgui_impl_vulkan.cpp"
//#include "3rd/imgui/imgui_impl_wgpu.cpp"
#include "3rd/imgui/imgui_impl_win32.cpp"
