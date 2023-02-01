#pragma once

#include <sol/force_inline.h>

#include "lauxlib.h"

#include <array>
#include <concepts>

namespace sol::lbind::detail {
    template <class Self>
    concept DefinesLuaName = requires {
        { Self::luaName } -> std::convertible_to<char const*>;
    };
}

namespace sol::lbind {
    template <class T>
    struct LuaName;

    template <detail::DefinesLuaName T>
    struct LuaName<T> {
        constexpr static inline decltype(auto) value = T::luaName;
    };

    template <class Self>
    concept Exported = alignof(Self) <= alignof(size_t) && requires {
        { LuaName<Self>::value } -> std::convertible_to<char const*>;
    };
}
