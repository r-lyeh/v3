#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    // Create a new Lua state
    lua_State *L = luaL_newstate();
    if (L == NULL) {
        printf("Failed to create Lua state\n");
        return 1;
    }

    // Open standard Lua libraries
    luaL_openlibs(L);

    // Lua script to print "Hello, World!"
    const char *lua_script = "print('Hello, World!')";

    // Load and execute the Lua script
    if (luaL_dostring(L, lua_script) != LUA_OK) {
        printf("Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from stack
    }

    // Clean up
    lua_close(L);
    return 0;
}
