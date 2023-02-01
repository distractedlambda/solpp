#pragma once

#include <sol/concepts/unwrappable.h>
#include <sol/lbind/lua_name.h>

#include "lauxlib.h"

#include <optional>
#include <string>
#include <string_view>
#include <variant>

namespace sol::lbind {
    template <class T>
    struct ConvertArgument;

    template <class Self>
    concept ArgumentConvertible = requires (lua_State* state, int& index) {
        { ConvertArgument<Self>::convert(state, index) } -> std::convertible_to<Self>;
        { ConvertArgument<Self>::tryConvert(state, index) } -> UnwrappableTo<Self>;
    };

    template <std::integral T>
    struct ConvertArgument<T> {
        SOL_FORCE_INLINE
        static T convert(lua_State* state, int& index) {
            auto value = luaL_checkinteger(state, index);
            luaL_argcheck(state, std::in_range<T>(value), index++, "value out of range");
            return static_cast<T>(value);
        }

        SOL_FORCE_INLINE
        static std::optional<T> tryConvert(lua_State* state, int& index) {
            int wasInteger;
            auto value = lua_tointegerx(state, index, &wasInteger);
            if (wasInteger && std::in_range<T>(value)) {
                index++;
                return static_cast<T>(value);
            } else {
                return std::nullopt;
            }
        }
    };

    template <std::floating_point T>
    struct ConvertArgument<T> {
        SOL_FORCE_INLINE
        static T convert(lua_State* state, int& index) {
            return static_cast<T>(luaL_checknumber(state, index++));
        }

        SOL_FORCE_INLINE
        static std::optional<T> tryConvert(lua_State* state, int& index) {
            int wasNumber;
            auto value = lua_tonumberx(state, index, &wasNumber);
            if (wasNumber) {
                index++;
                return static_cast<T>(value);
            } else {
                return std::nullopt;
            }
        }
    };

    template <>
    struct ConvertArgument<std::string_view> {
        SOL_FORCE_INLINE
        static std::string_view convert(lua_State* state, int& index) {
            size_t length;
            auto data = luaL_checklstring(state, index++, &length);
            return {data, length};
        }

        SOL_FORCE_INLINE
        static std::optional<std::string_view> tryConvert(lua_State* state, int& index) {
            size_t length;
            if (auto* chars = lua_tolstring(state, index, &length)) {
                index++;
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
        static char const* convert(lua_State* state, int& index) {
            return luaL_checkstring(state, index++);
        }

        SOL_FORCE_INLINE
        static std::optional<char const*> tryConvert(lua_State* state, int& index) {
            if (auto* chars = lua_tostring(state, index)) {
                index++;
                return chars;
            } else {
                return std::nullopt;
            }
        }
    };

    template <Exported T>
    struct ConvertArgument<T*> {
        SOL_FORCE_INLINE
        static T* convert(lua_State* state, int& index) {
            if (lua_isnoneornil(state, index)) {
                index++;
                return nullptr;
            } else {
                return static_cast<T*>(luaL_checkudata(state, index++, LuaName<T>::value));
            }
        }

        SOL_FORCE_INLINE
        static std::optional<T*> tryConvert(lua_State* state, int& index) {
            if (lua_isnoneornil(state, index)) {
                index++;
                return nullptr;
            } else if (auto* mem = luaL_testudata(state, index, LuaName<T>::value)) {
                index++;
                return static_cast<T*>(mem);
            } else {
                return std::nullopt;
            }
        }
    };

    template <Exported T>
    struct ConvertArgument<T&> {
        SOL_FORCE_INLINE
        static T& convert(lua_State* state, int& index) {
            return *static_cast<T*>(luaL_checkudata(state, index++, LuaName<T>::value));
        }

        SOL_FORCE_INLINE
        static T* tryConvert(lua_State* state, int& index) {
            if (auto* mem = luaL_testudata(state, index, LuaName<T>::value)) {
                index++;
                return *static_cast<T*>(mem);
            } else {
                return nullptr;
            }
        }
    };

    template <Exported T>
    struct ConvertArgument<T const&> {
        SOL_FORCE_INLINE
        static T const& convert(lua_State* state, int& index) {
            return ConvertArgument<T&>::convert(state, index);
        }

        SOL_FORCE_INLINE
        static T const* tryConvert(lua_State* state, int& index) {
            return ConvertArgument<T&>::tryConvert(state, index);
        }
    };

    template <ArgumentConvertible T>
    struct ConvertArgument<std::optional<T>> {
        SOL_FORCE_INLINE
        static std::optional<T> convert(lua_State* state, int& index) {
            if (lua_isnoneornil(state, index)) {
                index++;
                return std::nullopt;
            } else {
                return {std::in_place, ConvertArgument<T>::convert(state, index)};
            }
        }

        SOL_FORCE_INLINE
        static std::optional<std::optional<T>> tryConvert(lua_State* state, int& index) {
            if (lua_isnoneornil(state, index)) {
                index++;
                return {std::in_place, std::nullopt};
            } else if (auto&& converted = ConvertArgument<T>::tryConvert(state, index)) {
                return {std::in_place, *std::forward<decltype(converted)>(converted)};
            } else {
                return std::nullopt;
            };
        }
    };

    template <ArgumentConvertible... Ts>
    struct ConvertArgument<std::variant<Ts...>> {
        SOL_FORCE_INLINE
        static std::variant<Ts...> convert(lua_State* state, int& index) {
            return ConvertArgument::convertImpl<0, Ts...>(state, index);
        }

        SOL_FORCE_INLINE
        static std::optional<std::variant<Ts...>> tryConvert(lua_State* state, int& index) {
            return ConvertArgument::tryConvertImpl<0, Ts...>(state, index);
        }

    private:
        template <size_t> [[noreturn]] SOL_FORCE_INLINE
        static std::variant<Ts...> convertImpl(lua_State* state, int& index) {
            luaL_argerror(state, index, "invalid type");
        }

        template <size_t I, class U, class... Us> SOL_FORCE_INLINE
        static std::variant<Ts...> convertImpl(lua_State* state, int& index) {
            if (auto&& converted = ConvertArgument<U>::tryConvert(state, index)) {
                return {std::in_place_index<I>, *std::forward<decltype(converted)>(converted)};
            } else {
                return ConvertArgument::convertImpl<I + 1, Us...>(state, index);
            }
        }

        template <size_t> SOL_FORCE_INLINE
        static std::optional<std::variant<Ts...>> tryConvertImpl(lua_State*, int&) {
            return std::nullopt;
        }

        template <size_t I, class U, class... Us> SOL_FORCE_INLINE
        static std::optional<std::variant<Ts...>> tryConvertImpl(lua_State* state, int& index) {
            if (auto&& converted = ConvertArgument<U>::tryConvert(state, index)) {
                return {std::in_place, std::in_place_index<I>, *std::forward<decltype(converted)>(converted)};
            } else {
                return ConvertArgument::tryConvertImpl<I + 1, Us...>();
            }
        }
    };
}
