#pragma once

#include <sol/force_inline.h>

#include <concepts>

namespace sol::lbind {
    template <class Self>
    concept DefinesLuaName = requires {
        { Self::luaName() } -> std::convertible_to<char const*>;
    };

    template <class T>
    struct ExportInfo;

    template <DefinesLuaName T>
    struct ExportInfo<T> {
        SOL_FORCE_INLINE
        static decltype(auto) name() {
            return T::luaName();
        }
    };

    template <class T>
    concept Exported = alignof(T) <= alignof(size_t) && requires {
        { ExportInfo<T>::name() } -> std::convertible_to<char const*>;
    };
}
