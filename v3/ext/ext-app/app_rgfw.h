#if CODE
#pragma once
#define RGFW_ALLOC_DROPFILES
#define RGFW_USE_XDL // feel free to remove this line if you don't want to use XDL (-lX11 -lXrandr -lGLX will be required)
#define RGFW_IMPLEMENTATION
#define RGFW_PRINT_ERRORS
//#define RGFW_ADVANCED_SMOOTH_RESIZE
//#define RGFW_DEBUG

RGFW_window *app;

void* app_handle_(void) {
    return app;
}
void app_icon_(const char *iconfile) {
#ifdef _WIN32
    if( strendi(iconfile, ".ico") ) {
        HWND hWnd = app_handle_native();
        HICON hIcon = LoadImageA(0, iconfile, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
        if( hWnd && hIcon ) {
            HINSTANCE hInstance = (HINSTANCE)GetModuleHandleA(NULL);
            SendMessage(hWnd, WM_SETICON, ICON_BIG,   (LPARAM)hIcon);
            SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            //SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG,   (LPARAM)hIcon);
            //SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
        return;
    }
#endif
    image_t img = image(iconfile, 0, 4);
    if( img.pixels ) {
        RGFW_window_setIconEx(app, RGFW_IMAGE(img.pixels, RGFW_AREA(img.w, img.h), RGFW_formatRGBA8), RGFW_iconBoth);
        image_free(&img);
    }
}
void app_destroy_(void) {
    if( app ) {
        RGFW_window_close(app);
        app = 0;
    }
}
void* app_create_(const char *title, float scale) {
    assert( scale > 0 );
    while( scale > 1.1 ) scale /= 100; // keep it (0..1]

    if( !app ) {
        RGFW_init();

        RGFW_area desktop = //RGFW_getScreenSize();
            RGFW_getPrimaryMonitor().mode.area;
        RGFW_area rect = desktop; rect.w *= scale; rect.h *= scale;
        RGFW_area topleft = {(desktop.w - rect.w) / 2, (desktop.h - rect.h) / 3};

#ifndef GL2
//        RGFW_setHint_OpenGL(RGFW_glMajor, 3);
//        RGFW_setHint_OpenGL(RGFW_glMinor, 3);
//        RGFW_setHint_OpenGL(RGFW_glProfile, RGFW_glCore);
#endif

        app = RGFW_createWindow(title ? title : "", RGFW_RECT(topleft.w, topleft.h, rect.w, rect.h), 0);

#ifndef GL2
        if( app ) {
            RGFW_window_makeCurrentContext_OpenGL(app);

            #ifndef RGFW_WASM
            if (!gladLoadGL((GLADloadfunc)RGFW_getProcAddress_OpenGL)) {
                die("Failed to initialize GL\n");
            }
            #endif
        }
#endif

#if 1 // fullscreen
        if( app )
        if( scale > 1 ) {
            RGFW_monitor mon = RGFW_window_getMonitor(app);
            RGFW_monitor_scaleToWindow(mon, app);
            RGFW_window_setFullscreen(app, 1);
            RGFW_window_raise(app);
        }
#endif

        if( app ) { ONCE atexit(app_destroy_); }
        if( app ) { ONCE printf("OpenGL %s\n", (const char*)glGetString(GL_VERSION)); }
        if( app ) { RGFW_window_setExitKey(app, 0); }
        if( app ) { glClearColor(0.12f, 0.12f, 0.12f, 1.0f); }
    }

    return app;
}
int app_width_() {
    return app ? RGFW_window_getRect(app).w : 0;
}
int app_height_() {
    int h = app ? RGFW_window_getRect(app).h : 0;
    return h + !h;
}
int app_swap_() {
    if(!app) return 0;
    if(RGFW_window_shouldClose(app) == RGFW_TRUE) return 0;

    static int ever = 0; if(!ever) ever = 1; else { // disabled on first frame, enabled for any other frame
        api_end();
        RGFW_window_swapBuffers_OpenGL(app);
    }

    glViewport(0,0,app_width_(),app_height_());
    glClear(GL_COLOR_BUFFER_BIT); //< probably not needed in most apps

    api_begin();

    return 1; // keep going
}

#endif

#include "3rd/rgfw.h"

#if CODE
void* app_handle_native_(void) {
    return app ? (void*)app->src.window : (void*)0;
}
#endif
