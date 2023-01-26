#pragma once

#include <sol/force_inline.h>

#include <array>
#include <concepts>
#include <cstddef>
#include <utility>

namespace sol {
    template <class T, size_t N>
    class Vec {
        template <class, size_t>
        friend class Vec;

    public:
        SOL_FORCE_INLINE
        constexpr Vec() = default;

        SOL_FORCE_INLINE
        constexpr Vec(Vec const&) = default;

        SOL_FORCE_INLINE
        constexpr Vec& operator=(Vec const&) = default;

        SOL_FORCE_INLINE
        constexpr ~Vec() = default;

        SOL_FORCE_INLINE
        constexpr Vec(T c): Vec(broadcast_init_t(), c, std::make_index_sequence<N>()) {}

        SOL_FORCE_INLINE
        constexpr Vec(auto... args)
        requires (N > 1 && sizeof...(args) == N): _elements{args...} {}

        template <class U> SOL_FORCE_INLINE
        constexpr Vec(Vec<U, N> other): Vec(convert_init_t(), other, std::make_index_sequence<N>()) {}

        template <std::invocable<T const&> F> [[nodiscard]] SOL_FORCE_INLINE
        constexpr auto map(F func) const {
            return std::apply([&](auto... vs) { return Vec(std::invoke(func, vs)...); }, _elements);
        }

    private:
        std::array<T, N> _elements;

        struct broadcast_init_t {};

        template <size_t... I> SOL_FORCE_INLINE
        constexpr Vec(broadcast_init_t, T const& c, std::index_sequence<I...>): _elements{(I, c)...} {}

        struct convert_init_t {};

        template <class U, size_t... I> SOL_FORCE_INLINE
        constexpr Vec(convert_init_t, Vec<U, N> other, std::index_sequence<I...>): _elements{other._elements[I]...} {}
    };

    template <class... Args>
    Vec(Args...) -> Vec<std::common_type_t<Args...>, sizeof...(Args)>;

    #define SOL_VEC_UNARY_OP(func) \
        template <class T, size_t N> [[nodiscard]] SOL_FORCE_INLINE \
        auto func(Vec<T, N> v) { \
            return v.map([](auto c) { return func(c); }); \
        }

    SOL_VEC_UNARY_OP(abs);

    #undef SOL_VEC_UNARY_OP
}
