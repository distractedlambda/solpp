#pragma once

#include <sol/lbind/lua_name.h>
#include <sol/lbind/wrapper_function.h>
#include <sol/panic.h>

namespace sol::lbind::detail {
    void registerTypeMetatable(lua_State* state, char const* type);

    void registerMetamethod(lua_State* state, char const* type, char const* name, lua_CFunction body);

    void registerGetter(lua_State* state, char const* type, char const* name, lua_CFunction body);

    void registerSetter(lua_State* state, char const* type, char const* name, lua_CFunction body);

    void registerMethod(lua_State* state, char const* type, char const* name, lua_CFunction body);
}

namespace sol::lbind {
    template <Exported T>
    void registerType(lua_State* state) {
        luaL_newmetatable(state, LuaName<T>::value);

        if constexpr (std::is_destructible_v<T> && !std::is_trivially_destructible_v<T>) {
            lua_pushstring(state, "__gc");
            lua_pushcfunction(state, detail::wrappedFunction<&T::~T()>);
            lua_rawset(state, -3);
        }

        lua_pop(state, 1);
    }
}
