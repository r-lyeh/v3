void call_(callback_t c) {
    if(c.func0) c.func0();
    if(c.func1) c.func1(c.arg0);
    if(c.func2) c.func2(c.arg0, c.arg1);
}
