#include <sol/guard.h>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include "lauxlib.h"

int main(int argc, char** argv) {
    CLI::App app("Sol");

    CLI11_PARSE(app, argc, argv);

    auto luaState = luaL_newstate();
    sol::Guard destroyLuaState = [&]{ lua_close(luaState); };

    return 0;
}
