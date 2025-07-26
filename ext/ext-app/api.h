extern void*  (*app_create)(const char *title, const char *flags);  // flags: SCALE.f GL(M)(N) (+)core (!)debug
extern  int   (* app_swap)(void);
extern  int   (* app_width)(void);
extern  int   (* app_height)(void);
extern float  (* app_framerate)(void);
extern char*  (* app_flags)(void);
extern void*  (* app_handle)(void); // handle to windowing library (SDL_Window, GLFWWindow, etc)
extern void*  (* app_handle_native)(void); // handle to native os window (HWND, etc)
extern void   (* app_show)(bool);
extern void   (* app_icon)(const char *iconfile);
extern void*  (* app_proc)(const char *funcname);
extern void   (*app_destroy)(void);

extern void   on_context_created(void *fn, void *arg);
extern void   on_frame_begin(void *fn, void *arg);
extern void   on_frame_end(void *fn, void *arg);

#if CODE
#pragma once
#include v3_app_backend

void on_context_created(void *fn, void *arg) { hook(on_context_created, fn, arg); }
void on_frame_begin(void *fn, void *arg) { hook(on_frame_begin, fn, arg); }
void on_frame_end(void *fn, void *arg) { hook(on_frame_end, fn, arg); }

void*  (*app_create)(const char *, const char *flags) = app_create_;
int    (* app_swap)(void) = app_swap_;
int    (* app_width)(void) = app_width_;
int    (* app_height)(void) = app_height_;
char*  (* app_flags)(void) = app_flags_;
float  (* app_framerate)(void) = app_framerate_;
void*  (* app_handle)(void) = app_handle_;
void*  (* app_handle_native)(void) = app_handle_native_;
void   (* app_icon)(const char *) = app_icon_;
void   (* app_show)(bool) = app_show_;
void*  (* app_proc)(const char *funcname) = app_proc_;
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
