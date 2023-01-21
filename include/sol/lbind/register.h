#pragma once

#include <sol/lbind/exports.h>
#include <sol/lbind/wrapper_function.h>

#include "lauxlib.h"

namespace sol::lbind {
    template <Exported T>
    void registerType(lua_State* state) {
        luaL_newmetatable(state, Exports<T>::name);

        if constexpr (std::is_destructible_v<T> && !std::is_trivially_destructible_v<T>) {
            lua_pushstring(state, "__gc");
            lua_pushcfunction(state, detail::wrappedFunction<&T::~T()>);
            lua_rawset(state, -3);
        }

        lua_pop(state, 1);
    }
}
