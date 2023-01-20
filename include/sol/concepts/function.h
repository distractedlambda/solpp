#pragma once

#include <type_traits>

namespace sol {
    template <class Self>
    concept Function = std::is_function_v<Self>;
}
