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

    template <class Self>
    concept DefinesLuaMethods = requires {
        Self::luaMethods;
    };
}

namespace sol::lbind {
    template <class T>
    struct Exports;

    template <detail::DefinesLuaName T>
    struct Exports<T> {
        constexpr static inline decltype(auto) name = T::luaName;
    };

    template <class Self>
    concept Exported = alignof(Self) <= alignof(size_t) && requires {
        { Exports<Self>::name } -> std::convertible_to<char const*>;
    };
}
