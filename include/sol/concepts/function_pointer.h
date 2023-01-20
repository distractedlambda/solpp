#pragma once

#include <sol/concepts/function.h>
#include <sol/concepts/pointer.h>

namespace sol {
    template <class Self>
    concept FunctionPointer = Pointer<Self> && Function<std::remove_pointer_t<Self>>;
}
