extern void*  (*app_create)(const char *title, float scale);
extern  int   (* app_swap)();
extern  int   (* app_width)();
extern  int   (* app_height)();
extern void*  (* app_handle)(void); // handle to windowing library (SDL_Window, GLFWWindow, etc)
extern void*  (* app_handle_native)(void); // handle to native os window (HWND, etc)
extern void   (* app_icon)(const char *iconfile);
extern void   (*app_destroy)(void);

#include v3_app_backend

#if CODE
#pragma once

void*  (*app_create)(const char *title, float scale) = app_create_;
int    (* app_swap)() = app_swap_;
int    (* app_width)() = app_width_;
int    (* app_height)() = app_height_;
void*  (* app_handle)(void) = app_handle_;
void*  (* app_handle_native)(void) = app_handle_native_;
void   (* app_icon)(const char *iconfile) = app_icon_;
void   (*app_destroy)(void) = app_destroy_;

#ifdef _WIN32
// Grabs icon from main window and applies it to every newly created window (including assertions and dialog boxes)
static LRESULT WINAPI window_create_callback(int type, WPARAM wparam, LPARAM lparam) {
    if (type == HCBT_CREATEWND) {
        for( HWND hWnd = (HWND)app_handle_native(); hWnd; hWnd = NULL ) {
            // GetWindowIcon from parent window
            HICON hIcon = (HICON)SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);
            if(!hIcon) hIcon = (HICON)GetClassLongPtr(hWnd, GCLP_HICON);
            if(!hIcon) hIcon = (HICON)GetClassLongPtr(hWnd, GCLP_HICONSM);
            // Apply hIcon to newer window
            SendMessage((HWND)wparam, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessage((HWND)wparam, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
    }
    return CallNextHookEx(NULL, type, wparam, lparam);
}
AUTORUN {
    SetWindowsHookEx(WH_CBT, (HOOKPROC)window_create_callback, NULL, GetCurrentThreadId());
}
#endif

#endif
