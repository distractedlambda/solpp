#pragma once

#include <memory>

namespace sol {
    template <auto V>
    struct Constant {
        using value_type = decltype(V);

        constexpr static inline value_type value = V;

        consteval operator value_type() const {
            return V;
        }

        consteval value_type operator*() const {
            return V;
        }

        consteval value_type const* operator->() const {
            return std::addressof(V);
        }
    };

    template <auto V>
    constexpr inline Constant<V> constant;
}
