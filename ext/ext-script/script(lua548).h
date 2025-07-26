#if CODE
#pragma once

lua_State* script_create_() {
    lua_State *L = luaL_newstate();
    if( L ) luaL_openlibs(L);
    return L;
}
void script_destroy_(lua_State *L) {
    lua_close(L);
}
bool script_runfile_(lua_State *L, const char *fname) {
    int status = luaL_loadfile(L, fname);
    if( status ) return perror("cannot load file"), 0;
    int result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if( result ) return perror("cannot run script"), perror(lua_tostring(L, -1)), 0;
    return 1;
}
bool script_runstring_(lua_State *L, const char *str) {
    luaL_loadstring(L, str);
    lua_call(L, 0, 0);
    return 1;
}
bool script_preload_(lua_State *L, const char *modname, const char *modcode) {
    // usage:
    // lua_preload(L, "A", "num = 3, p = print"); // create table as module A, with members in it
    // luaL_dostring(L, "local A = require 'A'\nA.p(A.num)"); // read it from other script
    char *code = va("package.preload['%s'] = function()\n"
                    "return { %s } end\n", modname, modcode);
    return !luaL_dostring(L, code);
}
#endif
