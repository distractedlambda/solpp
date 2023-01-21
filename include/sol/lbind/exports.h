#pragma once

#include <sol/force_inline.h>

#include "lauxlib.h"

#include <array>
#include <concepts>

namespace sol::lbind::detail {
    enum class BindingKind {
        Metamethod,
        Method,
        PropertyGetter,
        PropertySetter,
    };

    template <BindingKind KIND, auto F>
    struct Binding {
        char const* key;
    };

    template <class Self>
    concept DefinesOwnExports = requires {
        { Self::luaName } -> std::convertible_to<char const*>;
        Self::luaBindings;
    };
}

namespace sol::lbind {
    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> add() {
        return {"__add"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> sub() {
        return {"__sub"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> mul() {
        return {"__mul"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> div() {
        return {"__div"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> mod() {
        return {"__mod"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> pow() {
        return {"__pow"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> unm() {
        return {"__unm"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> idiv() {
        return {"__idiv"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> band() {
        return {"__band"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> bor() {
        return {"__bor"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> bxor() {
        return {"__bxor"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> bnot() {
        return {"__bnot"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> shl() {
        return {"__shl"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> shr() {
        return {"__shr"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> concat() {
        return {"__concat"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> len() {
        return {"__len"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> eq() {
        return {"__eq"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> lt() {
        return {"__lt"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> le() {
        return {"__le"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> index() {
        return {"__index"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> newindex() {
        return {"__newindex"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Metamethod, F> call() {
        return {"__call"};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::Method, F> method(char const* name) {
        return {name};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::PropertyGetter, F> get(char const* name) {
        return {name};
    }

    template <auto F>
    consteval detail::Binding<detail::BindingKind::PropertySetter, F> set(char const* name) {
        return {name};
    }

    template <class T>
    struct Exports;

    template <detail::DefinesOwnExports T>
    struct Exports<T> {
        constexpr static inline decltype(auto) name = T::luaName;
        constexpr static inline decltype(auto) bindings = T::luaBindings;
    };

    template <class Self>
    concept Exported = alignof(Self) <= alignof(size_t) && requires {
        { Exports<Self>::name } -> std::convertible_to<char const*>;
        Exports<Self>::bindings;
    };
}
