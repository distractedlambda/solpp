#pragma once

#include <concepts>
#include <tuple>

namespace sol {
    template <class Self>
    concept TupleLike = requires {
        { std::tuple_size_v<Self> } -> std::same_as<size_t>;
    };
}
