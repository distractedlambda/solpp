#pragma once

#include <type_traits>

namespace sol {
    template <class Self>
    concept Pointer = std::is_pointer_v<Self>;
}
