void alert2_(const char *message, const char *caption) {
    // app_show(0);

    char joint[1024], *bt = message[0] == '!' ? trace(+16) : NULL;
    snprintf(joint, 1024, "%s\n%s\n", message + (message[0] == '!'), bt ? bt : "");

#if V3_WINDOWS
    MessageBoxA(0, joint, caption,0);
#elif V3_WASM
    emscripten_run_script(va("alert('%s')", joint));
#elif V3_LINUX
    for(FILE *fp = fopen("/tmp/.warning","wb");fp;fp=0)
    fputs(joint,fp), fclose(fp), system("xmessage -center -file /tmp/.warning");
#elif V3_MACOS
    system(va("osascript -e 'display alert \"%s\" message \"%s\"'", caption, joint));
#endif

    // app_show(1);
}
void alert_(const char *message) {
    alert2_(message, "Warning");
}
