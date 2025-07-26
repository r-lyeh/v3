int system_async( const char *cmd ) {
    if( !cmd[0] ) return false;
#if 0
    cmd = file_normalize(cmd);
#endif
#if _WIN32
    bool ok = WinExec(va("cmd /c \"%s\"", cmd), SW_HIDE) > 31;
#else
    bool ok = system(va("%s &", cmd)) == 0;
#endif
    return ok;
}

void beep_(void) {
    ifdef(V3_WINDOWS, system_async("rundll32 user32.dll,MessageBeep"); return; );
    ifdef(V3_LINUX, system_async("paplay /usr/share/sounds/freedesktop/stereo/message.oga"); return; );
    ifdef(V3_MACOS,   system_async("tput bel"); return; );

    //fallback:
    fputc('\x7', stdout);

    // win32:
    // rundll32 user32.dll,MessageBeep ; ok
    // rundll32 cmdext.dll,MessageBeepStub ; ok

    // osx:
    // tput bel
    // say "beep"
    // osascript -e 'beep'
    // osascript -e "beep 1"
    // afplay /System/Library/Sounds/Ping.aiff
    // /usr/bin/printf "\a"

    // linux:
    // paplay /usr/share/sounds/freedesktop/stereo/message.oga ; ok
    // paplay /usr/share/sounds/freedesktop/stereo/complete.oga ; ok
    // paplay /usr/share/sounds/freedesktop/stereo/bell.oga ; ok
    // beep ; apt-get
    // echo -e '\007' ; mute
    // echo -e "\007" >/dev/tty10 ; sudo
    // tput bel ; mute
}
