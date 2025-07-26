#pragma comment(lib, "user32")

void alert_(alert_flags_t opt) {
    // app_show(0);

    const char *caption = strvalid(opt.title);
    const char *message = strvalid(opt.message);

    int debug = caption[0] == '!' || message[0] == '!';

    caption += caption[0] == '!';
    message += message[0] == '!';

    char joint[1024];
    snprintf(joint, 1024, "%s\n%s\n", message, debug ? trace(+16) : "");

#if V3_WINDOWS
    MessageBoxA(0, joint, caption, 0);
#elif V3_WASM
    emscripten_run_script(va("alert('%s')", joint));
#elif V3_LINUX
    for(FILE *fp = fopen("/tmp/.warning","wb"); fp; fp=0)
    fputs(joint, fp), fclose(fp), system("xmessage -center -file /tmp/.warning");
#elif V3_MACOS
    system(va("osascript -e 'display alert \"%s\" message \"%s\"'", caption, joint));
#endif

    // app_show(1);
}
