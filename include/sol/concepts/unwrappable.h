#pragma once

#include <concepts>

namespace sol {
    template <class Self>
    concept Unwrappable = requires (Self&& self) {
        static_cast<bool>(std::forward<Self>(self));
        *std::forward<Self>(self);
    };

    template <class Self, class U>
    concept UnwrappableTo = Unwrappable<Self> && requires (Self&& self) {
        { *std::forward<Self>(self) } -> std::convertible_to<U>;
    };
}
