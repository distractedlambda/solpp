#pragma once

#include <sol/force_inline.h>

#include <concepts>
#include <functional>
#include <tuple>

namespace sol::detail {
    template <class... F>
    class OverloadSet {
    public:
        template <class... Args> SOL_FORCE_INLINE
        constexpr OverloadSet(Args&&... args): _alternatives(std::forward<Args>(args)...) {}

        template <class... Args> SOL_FORCE_INLINE
        constexpr decltype(auto) operator()(Args&&... args) const& {
            return this->dispatch<0>(std::forward<Args>(args)...);
        }

        template <class... Args> SOL_FORCE_INLINE
        constexpr decltype(auto) operator()(Args&&... args) & {
            return this->dispatch<0>(std::forward<Args>(args)...);
        }

        template <class... Args> SOL_FORCE_INLINE
        constexpr decltype(auto) operator()(Args&&... args) && {
            return std::move(*this).template dispatch<0>(std::forward<Args>(args)...);
        }

    private:
        std::tuple<F...> _alternatives;

        template <size_t I, class... Args> SOL_FORCE_INLINE
        constexpr decltype(auto) dispatch(Args&&... args) const& {
            if constexpr (std::invocable<std::tuple_element_t<I, decltype(_alternatives)> const&, Args...>) {
                return std::invoke(std::get<I>(_alternatives), std::forward<Args>(args)...);
            } else {
                return this->dispatch<I + 1>(std::forward<Args>(args)...);
            }
        }

        template <size_t I, class... Args> SOL_FORCE_INLINE
        constexpr decltype(auto) dispatch(Args&&... args) & {
            if constexpr (std::invocable<std::tuple_element_t<I, decltype(_alternatives)>&, Args...>) {
                return std::invoke(std::get<I>(_alternatives), std::forward<Args>(args)...);
            } else {
                return this->dispatch<I + 1>(std::forward<Args>(args)...);
            }
        }

        template <size_t I, class... Args> SOL_FORCE_INLINE
        constexpr decltype(auto) dispatch(Args&&... args) && {
            if constexpr (std::invocable<std::tuple_element_t<I, decltype(_alternatives)>, Args...>) {
                return std::invoke(std::move(std::get<I>(_alternatives)), std::forward<Args>(args)...);
            } else {
                return std::move(*this).template dispatch<I + 1>(std::forward<Args>(args)...);
            }
        }
    };
}

namespace sol {
    template <class... Alts> SOL_FORCE_INLINE
    constexpr detail::OverloadSet<std::decay_t<Alts>...> overload(Alts&&... alternatives) {
        return {std::forward<Alts>(alternatives)...};
    }
}
