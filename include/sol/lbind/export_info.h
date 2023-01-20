#pragma once

#include <sol/force_inline.h>

#include <concepts>

namespace sol::lbind {
    template <class T>
    struct ExportInfo {
        SOL_FORCE_INLINE
        static char const* name() {
            return T::luaName();
        }
    };

    template <class T>
    concept Exported = alignof(T) <= alignof(size_t) && requires {
        { ExportInfo<T>::name() } -> std::convertible_to<char const*>;
    };
}
