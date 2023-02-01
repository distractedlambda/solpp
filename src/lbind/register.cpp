#include <sol/lbind/register.h>
#include <sol/unreachable.h>

namespace {
    constexpr auto PROPERTY_GETTERS_KEY = "__getters";
    constexpr auto PROPERTY_SETTERS_KEY = "__setters";
    constexpr auto METHODS_KEY = "__methods";
    constexpr auto INDEX_IMPL_KEY = "__indexImpl";
    constexpr auto NEWINDEX_IMPL_KEY = "__newindexImpl";

    int dispatchIndex(lua_State* state) {
        if (lua_type(state, 2) == LUA_TSTRING) {
            if (luaL_getmetafield(state, 1, PROPERTY_GETTERS_KEY) != LUA_TNIL) {
                lua_pushvalue(state, 2);

                if (lua_rawget(state, 3) != LUA_TNIL) {
                    auto getter = lua_tocfunction(state, 4);
                    lua_settop(state, 1);
                    return getter(state);
                }

                lua_settop(state, 2);
            }

            if (luaL_getmetafield(state, 1, METHODS_KEY) != LUA_TNIL) {
                lua_pushvalue(state, 2);

                if (lua_rawget(state, 3) != LUA_TNIL) {
                    return 1;
                }

                lua_settop(state, 2);
            }
        }

        if (luaL_getmetafield(state, 1, INDEX_IMPL_KEY) != LUA_TNIL) {
            auto impl = lua_tocfunction(state, 3);
            lua_settop(state, 2);
            return impl(state);
        }

        [[unlikely]];

        luaL_getmetafield(state, 1, "__name");
        auto typeName = lua_tostring(state, 3);

        if (lua_type(state, 2) == LUA_TSTRING) {
            luaL_error(
                state,
                "value of type \"%s\" does not have a readable member named \"%s\"",
                typeName,
                lua_tostring(state, 2)
            );
        } else {
            luaL_error(state, "value of type \"%s\" does not support indexed access", typeName);
        }

        sol::unreachable();
    }

    int dispatchNewindex(lua_State* state) {
        if (lua_type(state, 2) == LUA_TSTRING && luaL_getmetafield(state, 1, PROPERTY_SETTERS_KEY) != LUA_TNIL) {
            lua_pushvalue(state, 2);

            if (lua_rawget(state, 4) != LUA_TNIL) {
                auto setter = lua_tocfunction(state, 5);
                lua_copy(state, 3, 2);
                lua_settop(state, 2);
                return setter(state);
            }

            lua_settop(state, 3);
        }

        if (luaL_getmetafield(state, 1, NEWINDEX_IMPL_KEY) != LUA_TNIL) {
            auto impl = lua_tocfunction(state, 4);
            lua_settop(state, 3);
            return impl(state);
        }

        [[unlikely]];

        luaL_getmetafield(state, 1, "__name");
        auto typeName = lua_tostring(state, 4);

        if (lua_type(state, 2) == LUA_TSTRING) {
            luaL_error(
                state,
                "value of type \"%s\" does not have an assignable member named \"%s\"",
                typeName,
                lua_tostring(state, 2)
            );
        } else {
            luaL_error(state, "value of type \"%s\" does not support indexed assignment", typeName);
        }

        sol::unreachable();
    }
}
