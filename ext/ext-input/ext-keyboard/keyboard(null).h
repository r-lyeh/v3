int    key_(int vk) { return 0; }
int    keyrepeat_(int vk) { return 0; }
int*   keyboard_(int vk) { static int kb[512*2]; return kb; }

unsigned *keytext_(unsigned *count) { *count = 0; return NULL; }
