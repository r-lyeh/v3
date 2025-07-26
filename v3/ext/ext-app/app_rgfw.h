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
    file_t f = file(iconfile);
    if( f.bin && f.len ) ; else return;

#ifdef _WIN32
    if( strendi(f.url, ".ico") ) {
        HWND hWnd = app_handle_native();
#if 0   // create from file
        HICON hIcon = LoadImageA(0, iconfile, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
#else   // create from memory
        struct ICONDIR {
            WORD reserved;     // 0
            WORD type;         // 1 for icons
            WORD count;        // Number of images
        } *dir = (struct ICONDIR*)f.bin;
        struct ICONDIRENTRY {
            BYTE width;        // Width (0 = 256)
            BYTE height;       // Height (0 = 256)
            BYTE colorCount;   // Number of colors (0 if >=8bpp)
            BYTE reserved;     // 0
            WORD planes;       // Color planes
            WORD bitCount;     // Bits per pixel
            DWORD bytesInRes;  // Size of image data
            DWORD imageOffset; // Offset to image data
        } *entry = (struct ICONDIRENTRY*)((BYTE*)f.bin + 6), *largest = entry;
        HICON hIcon = 0;
        // Read ICONDIR header
        if (dir->type == 1 && dir->count) {
            // Find largest image
            for( int i = 0, max_size = 0; i < dir->count; i++, entry++ ) {
                int width = entry->width == 0 ? 256 : entry->width;
                int height = entry->height == 0 ? 256 : entry->height;
                int size = width * height;
                if (size > max_size) {
                    max_size = size;
                    largest = entry;
                }
            }
            // Create icon from largest image
            hIcon = CreateIconFromResourceEx((BYTE*)f.bin + largest->imageOffset, largest->bytesInRes, TRUE, 0x00030000, 0, 0, LR_DEFAULTSIZE);
        }
#endif
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
    image_t i = image(.bin = f.bin, .len = f.len, .channels = 4);
    RGFW_window_setIconEx(app, RGFW_IMAGE(i.data, RGFW_AREA(i.w, i.h), RGFW_formatRGBA8), RGFW_iconBoth);
    drop(&i);
}
void app_destroy_(void) {
    if( app ) {
        RGFW_window_close(app);
        app = 0;
    }
}
void* app_create_(const char *title, const char *flags) {
    errno = 0;

    float scale = flags ? atof(flags) : 0.f;
    assert( scale > 0 );
    while( scale > 1.1 ) scale /= 100; // keep it (0..1]

    if( !app ) {
        RGFW_init();

        RGFW_area desktop = //RGFW_getScreenSize();
            RGFW_getPrimaryMonitor().mode.area;
        RGFW_area size = desktop; size.w *= scale; size.h *= scale;
        RGFW_area xy = {(desktop.w - size.w) / 2, (desktop.h - size.h) / 3};

#ifndef GL2
        extern const char *GL_hints(void);
        if( strbegi(GL_hints(), "GL") )
            RGFW_setHint_OpenGL(RGFW_glMajor, GL_hints()[2] - '0'),
            RGFW_setHint_OpenGL(RGFW_glMinor, GL_hints()[3] - '0'),
            RGFW_setHint_OpenGL(RGFW_glProfile, GL_hints()[4] == '+' ? RGFW_glCore : RGFW_glCompatibility); // RGFW_glES
            /* @todo: debug context if '!' */
#endif

        app = RGFW_createWindow(title ? title : "", RGFW_RECT(xy.w, xy.h, size.w, size.h), 0);

#ifndef GL2
        if( app ) {
            RGFW_window_makeCurrentContext_OpenGL(app);

            #ifndef RGFW_WASM
            extern bool GL_load(void *);
            if (!GL_load(RGFW_getProcAddress_OpenGL)) {
                die("Failed to initialize GL\n");
            }
            #endif
        }
#endif
        if( app ) { printf("OpenGL %s\n", (const char*)glGetString(GL_VERSION)); }
        if( app ) { glClearColor(0.12f, 0.12f, 0.12f, 1.0f); }

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
        if( app ) { RGFW_window_setExitKey(app, 0); }
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
