#if CODE
#pragma once
#define RGFW_ALLOC_DROPFILES
#define RGFW_USE_XDL // feel free to remove this line if you don't want to use XDL (-lX11 -lXrandr -lGLX will be required)
#define RGFW_IMPLEMENTATION
#define RGFW_PRINT_ERRORS
#define RGFW_OPENGL
//#define RGFW_ADVANCED_SMOOTH_RESIZE
//#define RGFW_DEBUG

#include "3rd/RGFW.h"

RGFW_window *app;
char *app_flags_inuse;

void* app_handle_(void) {
    return app;
}
void app_icon_(const char *iconfile) {
    file_t f = asset(iconfile);
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
    RGFW_window_setIconEx(app, i.data, i.w, i.h, RGFW_formatRGBA8, RGFW_iconBoth);
    drop(&i);
}
void app_destroy_(void) {
    if( app ) {
        RGFW_window_close(app);
        app = 0;
    }
}
char* app_flags_(void) {
    return app_flags_inuse;
}

// default flags create:
// - Highest GL version available by your gpu/driver.
// - Highest depth buffer available. Typically 32 or 24-bit.
// - GL debug context. Disabled in retail builds.
// - MSAA8 context. Use glEnable(GL_MULTISAMPLE) if needed.
// - Core context. Pass "Compat*" to change it.
// - Linear rgb. Pass "SRGB" to change it.
void* app_create_(const char *title, const char *flags) {
    title = title ? title : ""; puts(title);
    flags = flags ? flags : "";

    if( app_flags_inuse ) free(app_flags_inuse);
    app_flags_inuse = strdup(flags);

    float scale = flags ? atof(flags) : 0.f;
    assert( scale > 0 );
    while( scale > 1.1 ) scale /= 100; // keep it (0..1]

    if( !app ) {
        RGFW_init();

        RGFW_glHints *h = RGFW_getGlobalHints_OpenGL();
            h->debug = ifdef(V3_RETAIL,!!strstri(flags, "Debug"),1);
            h->profile = strstri(flags, "Compat") ? RGFW_glCompatibility : RGFW_glCore;
            h->samples = strstri(flags, "MSAA") && atoi(strstri(flags, "MSAA")+4) ? atoi(strstri(flags, "MSAA")+4) : 8;
            h->stencil = 8; // osx
            h->depth = 32; // try highest first
            h->sRGB = !!strstri(flags, "SRGB");
            /**/ if( strstri(flags, "GLES") ) h->profile = RGFW_glES;
            else if( strstri(flags, "GL2") )  h->profile = RGFW_glCompatibility, h->debug = 0;
            else if( strstri(flags, "GL3") || strstri(flags, "GL4") ) {
                if( strstri(flags, "GL")[3] > 0 )
                h->minor = strstri(flags, "GL")[3] - '0';
                h->major = strstri(flags, "GL")[2] - '0';
            }
            if(V3_MACOS) h->major = 3, h->minor = 2, h->stencil = 8, h->profile = RGFW_glCompatibility;
        RGFW_setGlobalHints_OpenGL(h);

        int desktop[] = {RGFW_getPrimaryMonitor().mode.w,RGFW_getPrimaryMonitor().mode.h};
        int wh[] = { desktop[0] * scale, desktop[1] * scale };
        int xy[] = {(desktop[0] - wh[0]) / 2, (desktop[1] - wh[1]) / 3};
        int rgfwFlags = 0|RGFW_windowHide|RGFW_windowOpenGL;

        for(;;) {
            app = RGFW_createWindow(title ? title : "", xy[0], xy[1], wh[0], wh[1], rgfwFlags);

            if( !app ) {
                die(va("Failed to create window: %s", flags));
            } else {
                RGFW_window_makeCurrentContext_OpenGL(app);
                event(on_context_created);
            }

            GLint depth_bits = 0;
            if(h->depth == (glGetIntegerv(GL_DEPTH_BITS, &depth_bits), depth_bits)) break;
            h->depth -= 8;
            RGFW_setGlobalHints_OpenGL(h);

            RGFW_window_close(app);
        }

        if( app ) {
            RGFW_window_show(app);

            // Query actual status
            GLint samples=0, sample_buffers=0, depth_bits=0, stencil_bits = 0;
            GLboolean srgb_enabled = glIsEnabled(GL_FRAMEBUFFER_SRGB);
            glGetIntegerv(GL_SAMPLES, &samples);
            glGetIntegerv(GL_SAMPLE_BUFFERS, &sample_buffers);
            glGetIntegerv(GL_DEPTH_BITS, &depth_bits);
            glGetIntegerv(GL_STENCIL_BITS, &stencil_bits);

            lputs(va("GPU device: %s", (const char*)glGetString(GL_RENDERER)));
            lputs(va("GPU driver: %s", (const char*)glGetString(GL_VERSION)));
            lputs(va("GPU compat: %d, depth: %d, stencil: %d, aa: %d, srgb: %d", h->profile, depth_bits, stencil_bits, samples, srgb_enabled));

            glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
            ( strstri(flags, "MSAA") ? glEnable : glDisable )(GL_MULTISAMPLE);
        }

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

    errno = 0;
    return app;
}
int app_width_(void) {
    int w = 0;
    if(app) RGFW_window_getSize(app, &w, 0);
    return w;
}
int app_height_(void) {
    int h = 0;
    if(app) RGFW_window_getSize(app, 0, &h);
    return h + !h;
}
int app_swap_(void) {
    if(!app) return 0;
    if(RGFW_window_shouldClose(app) == RGFW_TRUE) return 0;

    static int ever = 0; if(!ever) ever = 1; else { // disabled on first frame, enabled for any other frame
        event(on_frame_end);
        RGFW_window_swapBuffers_OpenGL(app);
    }

    glViewport(0,0,app_width_(),app_height_());
    glScissor(0,0,app_width_(),app_height_());
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //< probably not needed in most apps

    event(on_frame_begin);

    return 1; // keep going
}
void* app_proc_(const char *name) {
    return RGFW_getProcAddress_OpenGL(name);
}
float app_framerate_(void) {
    return RGFW_getPrimaryMonitor().mode.refreshRate;
}
void app_show_(bool on) {
    (on ? RGFW_window_show : RGFW_window_hide)(app);
}
void* app_handle_native_(void) {
    return app ? (void*)app->src.window : (void*)0;
}
#endif
