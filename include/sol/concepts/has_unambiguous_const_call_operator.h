#pragma once

#include <type_traits>

namespace sol::detail {
    template <class T>
    struct is_const_member_function_pointer: std::false_type {};

    template <class T, class Ret, class... Args>
    struct is_const_member_function_pointer<Ret (T::*)(Args...) const>: std::true_type {};

    template <class T>
    constexpr inline bool is_const_member_function_pointer_v = is_const_member_function_pointer<T>::value;
}

namespace sol {
    template <class Self>
    concept HasUnambiguousConstCallOperator = detail::is_const_member_function_pointer_v<decltype(&Self::operator())>;
}
