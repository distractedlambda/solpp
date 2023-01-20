#pragma once

#include <sol/lbind/wrapper_function.h>

namespace sol::lbind {
    template <auto F>
    constexpr inline lua_CFunction wrappedFunction = &detail::WrapperFunction<F>::invoke;
}
