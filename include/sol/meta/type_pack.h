#pragma once

#include <sol/force_inline.h>
#include <sol/meta/type.h>

#include <concepts>
#include <functional>

namespace sol {
    template <class... Ts>
    struct TypePack {
        constexpr static inline size_t size = sizeof...(Ts);

        template <std::invocable<Type<Ts>...> F>
        constexpr static decltype(auto) apply(F&& action) {
            return std::invoke(std::forward<F>(action), type<Ts>...);
        }
    };

    template <class... Ts>
    constexpr inline TypePack<Ts...> typePack;
}
