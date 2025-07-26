void die_(const char *msg) {
    perror(msg);
    alert_(msg);
    exit(-(fnv1a(msg, strlen(msg)) & 0x7fffffff));
}
