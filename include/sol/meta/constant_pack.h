#pragma once

#include <sol/force_inline.h>
#include <sol/meta/constant.h>

#include <concepts>
#include <functional>

namespace sol {
    template <auto... Vs>
    struct ConstantPack {
        constexpr static inline size_t size = sizeof...(Vs);

        template <std::invocable<Constant<Vs>...> F> SOL_FORCE_INLINE
        constexpr static decltype(auto) apply(F&& action) {
            std::invoke(std::forward<F>(action), constant<Vs>...);
        }
    };

    template <auto... Vs>
    constexpr inline ConstantPack<Vs...> constantPack;
}
