#include "stdafx.h"
#include "lua5.3.3/lua.hpp"

void print_lua_error(lua_State* state) {
    puts(lua_tostring(state, -1));
    lua_pop(state, 1);
}

int print_message(lua_State* state) {
    // Get argument count from off top of stack.
    int argc = lua_gettop(state);

    printf("I was called with %d arguments:\n", argc);

    for (size_t i = 1; i <= argc; ++i) {
        printf("  #%d: '%s'\n", (int)i, lua_tostring(state, (int)i));
    }

    // Push return value to stack.
    lua_pushnumber(state, 42);
    // Return number of return values pushed onto stack.
    return 1;
}

bool execute_script(const char* filename) {
    lua_State* state = luaL_newstate();

    luaL_openlibs(state);

    lua_register(state, "print_message", print_message);

    // Load LUA program.
    if (luaL_loadfile(state, filename) != LUA_OK) {
        print_lua_error(state);
        return false;
    }

    // Execute program.
    if (lua_pcall(state, 0, LUA_MULTRET, 0)) {
        print_lua_error(state);
        return false;
    }

    return true;
}

//int main() {
//	execute_script("../lua/hello.lua");
//	execute_script("../lua/world.lua");
//	execute_script("../lua/callback.lua");
//	//execute_script("lua/error.lua");
//
//	getchar();
//	return 0;
//}
