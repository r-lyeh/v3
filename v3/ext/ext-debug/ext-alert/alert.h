void alert2_(const char *message, const char *caption) {
    // app_show(0);

    char joint[1024], *bt = message[0] == '!' ? trace(+16) : NULL;
    snprintf(joint, 1024, "%s\n%s\n", message + (message[0] == '!'), bt ? bt : "");

#if DEFINED(win32)
    MessageBoxA(0, joint, caption,0);
#elif DEFINED(ems)
    emscripten_run_script(va("alert('%s')", joint));
#elif DEFINED(linux)
    for(FILE *fp = fopen("/tmp/.warning","wb");fp;fp=0)
    fputs(joint,fp), fclose(fp), system("xmessage -center -file /tmp/.warning");
#elif DEFINED(osx)
    system(va("osascript -e 'display alert \"%s\" message \"%s\"'", caption, joint));
#endif

    // app_show(1);
}
void alert_(const char *message) {
    alert2_(message, "Warning");
}
