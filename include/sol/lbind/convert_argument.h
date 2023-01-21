#pragma once

#include <sol/concepts/unwrappable.h>
#include <sol/lbind/exports.h>

#include "lauxlib.h"

#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace sol::lbind {
    template <class T>
    struct ConvertArgument;

    template <class Self>
    concept ArgumentConvertible = requires (lua_State* state, int arg) {
        { ConvertArgument<Self>::convert(state, arg) } -> std::convertible_to<Self>;
        { ConvertArgument<Self>::tryConvert(state, arg) } -> UnwrappableTo<Self>;
    };

    template <std::integral T>
    struct ConvertArgument<T> {
        SOL_FORCE_INLINE
        static T convert(lua_State* state, int arg) {
            auto value = luaL_checkinteger(state, arg);
            luaL_argcheck(state, std::in_range<T>(value), arg, "value out of range");
            return static_cast<T>(value);
        }

        SOL_FORCE_INLINE
        static std::optional<T> tryConvert(lua_State* state, int arg) {
            int wasInteger;
            auto value = lua_tointegerx(state, arg, &wasInteger);
            if (wasInteger && std::in_range<T>(value)) {
                return static_cast<T>(value);
            } else {
                return std::nullopt;
            }
        }
    };

    template <std::floating_point T>
    struct ConvertArgument<T> {
        SOL_FORCE_INLINE
        static T convert(lua_State* state, int arg) {
            return static_cast<T>(luaL_checknumber(state, arg));
        }

        SOL_FORCE_INLINE
        static std::optional<T> tryConvert(lua_State* state, int arg) {
            int wasNumber;
            auto value = lua_tonumberx(state, arg, &wasNumber);
            if (wasNumber) {
                return static_cast<T>(value);
            } else {
                return std::nullopt;
            }
        }
    };

    template <>
    struct ConvertArgument<std::string_view> {
        SOL_FORCE_INLINE
        static std::string_view convert(lua_State* state, int arg) {
            size_t length;
            auto data = luaL_checklstring(state, arg, &length);
            return {data, length};
        }

        SOL_FORCE_INLINE
        static std::optional<std::string_view> tryConvert(lua_State* state, int arg) {
            size_t length;
            if (auto* chars = lua_tolstring(state, arg, &length)) {
                return {{chars, length}};
            } else {
                return std::nullopt;
            }
        }
    };

    template <>
    struct ConvertArgument<std::string>: ConvertArgument<std::string_view> {};

    template <>
    struct ConvertArgument<char const*> {
        SOL_FORCE_INLINE
        static char const* convert(lua_State* state, int arg) {
            return luaL_checkstring(state, arg);
        }

        SOL_FORCE_INLINE
        static std::optional<char const*> tryConvert(lua_State* state, int arg) {
            if (auto* chars = lua_tostring(state, arg)) {
                return chars;
            } else {
                return std::nullopt;
            }
        }
    };

    template <Exported T>
    struct ConvertArgument<T*> {
        SOL_FORCE_INLINE
        static T* convert(lua_State* state, int arg) {
            if (lua_isnoneornil(state, arg)) {
                return nullptr;
            } else {
                return reinterpret_cast<T*>(luaL_checkudata(state, arg, Exports<T>::name));
            }
        }

        SOL_FORCE_INLINE
        static std::optional<T*> tryConvert(lua_State* state, int arg) {
            if (lua_isnoneornil(state, arg)) {
                return nullptr;
            } else if (auto* mem = luaL_testudata(state, arg, Exports<T>::name)) {
                return reinterpret_cast<T*>(mem);
            } else {
                return std::nullopt;
            }
        }
    };

    template <Exported T>
    struct ConvertArgument<T&> {
        SOL_FORCE_INLINE
        static T& convert(lua_State* state, int arg) {
            return *reinterpret_cast<T*>(luaL_checkudata(state, arg, Exports<T>::name));
        }

        SOL_FORCE_INLINE
        static T* tryConvert(lua_State* state, int arg) {
            return reinterpret_cast<T*>(luaL_testudata(state, arg, Exports<T>::name));
        }
    };

    template <Exported T>
    struct ConvertArgument<T const&> {
        SOL_FORCE_INLINE
        static T const& convert(lua_State* state, int arg) {
            return ConvertArgument<T&>::convert(state, arg);
        }

        SOL_FORCE_INLINE
        static T const* tryConvert(lua_State* state, int arg) {
            return ConvertArgument<T&>::tryConvert(state, arg);
        }
    };

    template <ArgumentConvertible T>
    struct ConvertArgument<std::optional<T>> {
        SOL_FORCE_INLINE
        static std::optional<T> convert(lua_State* state, int arg) {
            return luaL_opt(state, ConvertArgument<T>::convert, arg, std::optional<T>());
        }

        SOL_FORCE_INLINE
        static std::optional<std::optional<T>> tryConvert(lua_State* state, int arg) {
            return luaL_opt(state, ConvertArgument<T>::tryConvert, arg, std::optional<std::optional<T>>());
        }
    };

    template <ArgumentConvertible... Ts>
    struct ConvertArgument<std::variant<Ts...>> {
        SOL_FORCE_INLINE
        static std::variant<Ts...> convert(lua_State* state, int arg) {
            return ConvertArgument::convertImpl<0, Ts...>(state, arg);
        }

        SOL_FORCE_INLINE
        static std::optional<std::variant<Ts...>> tryConvert(lua_State* state, int arg) {
            return ConvertArgument::tryConvertImpl<0, Ts...>(state, arg);
        }

    private:
        template <size_t> [[noreturn]] SOL_FORCE_INLINE
        static std::variant<Ts...> convertImpl(lua_State* state, int arg) {
            luaL_argerror(state, arg, "invalid type");
        }

        template <size_t I, class U, class... Us> SOL_FORCE_INLINE
        static std::variant<Ts...> convertImpl(lua_State* state, int arg) {
            if (auto&& converted = ConvertArgument<U>::tryConvert(state, arg)) {
                return std::variant<Ts...>(std::in_place_index<I>, *std::forward<decltype(converted)>(converted));
            } else {
                return ConvertArgument::convertImpl<I + 1, Us...>(state, arg);
            }
        }

        template <size_t> SOL_FORCE_INLINE
        static std::optional<std::variant<Ts...>> tryConvertImpl(lua_State*, int) {
            return std::nullopt;
        }

        template <size_t I, class U, class... Us> SOL_FORCE_INLINE
        static std::optional<std::variant<Ts...>> tryConvertImpl(lua_State* state, int arg) {
            if (auto&& converted = ConvertArgument<U>::tryConvert(state, arg)) {
                return std::optional<std::variant<Ts...>>(std::in_place, *std::forward<decltype(converted)>(converted));
            } else {
                return ConvertArgument::tryConvertImpl<I + 1, Us...>();
            }
        }
    };
}
