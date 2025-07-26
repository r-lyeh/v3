void die_(const char *error, die_flags opt) {
    fprintf(stderr, "%s: %s (%s:%s:%d) %s\n", opt.title, error, opt.func, opt.file, opt.line, errno ? strerror(errno) : "");
    alert(opt.title, error); // warn user
    breakpoint(); // break-in if debugger is present
    exit(opt.retcode ? opt.retcode : -opt.line);
}
