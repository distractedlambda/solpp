#pragma once

#include <sol/lbind/export_info.h>

#include "lauxlib.h"

#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace sol::lbind {
    template <class T>
    struct ConvertResult;

    template <class Self>
    concept ResultConvertible = requires (lua_State* state, Self&& value) {
        ConvertResult<Self>::convert(state, std::forward<Self>(value));
    };

    template <std::integral T>
    struct ConvertResult<T> {
        SOL_FORCE_INLINE
        static void convert(lua_State* state, T value) {
            if (std::in_range<lua_Integer>(value)) {
                lua_pushinteger(state, static_cast<lua_Integer>(value));
            } else {
                lua_pushnumber(state, static_cast<lua_Number>(value));
            }
        }
    };

    template <std::floating_point T>
    struct ConvertResult<T> {
        SOL_FORCE_INLINE
        static void convert(lua_State* state, T value) {
            lua_pushnumber(state, static_cast<lua_Number>(value));
        }
    };

    template <>
    struct ConvertResult<std::string_view> {
        SOL_FORCE_INLINE
        static void convert(lua_State* state, std::string_view value) {
            lua_pushlstring(state, value.data(), value.size());
        }
    };

    template <>
    struct ConvertResult<std::string>: ConvertResult<std::string_view> {};

    template <>
    struct ConvertResult<char const*> {
        SOL_FORCE_INLINE
        static void convert(lua_State* state, char const* value) {
            lua_pushstring(state, value);
        }
    };

    template <ResultConvertible T>
    struct ConvertResult<std::optional<T>> {
        template <class Value> SOL_FORCE_INLINE
        static void convert(lua_State* state, Value&& value) {
            if (!value) {
                lua_pushnil(state);
            } else {
                ConvertResult<T>::convert(state, *std::forward<Value>(value));
            }
        }
    };

    template <Exported T>
    struct ConvertResult<T> {
        template <class Value> SOL_FORCE_INLINE
        static void convert(lua_State* state, Value&& value) {
            new (lua_newuserdatauv(state, sizeof(T), 0)) T(std::forward<Value>(value));
            luaL_setmetatable(state, ExportInfo<T>::name());
        }
    };

    template <ResultConvertible... Ts>
    struct ConvertResult<std::variant<Ts...>> {
        template <class Value> SOL_FORCE_INLINE
        static void convert(lua_State* state, Value&& value) {
            std::visit(
                [&]<class Arg>(Arg&& inhabitant) {
                    ConvertResult<std::remove_cvref_t<Arg>>::convert(state, std::forward<Arg>(inhabitant));
                },
                std::forward<Value>(value)
            );
        }
    };
}
