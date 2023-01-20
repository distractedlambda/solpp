#pragma once

#include <sol/concepts/has_unambiguous_call_operator.h>
#include <sol/concepts/tuple_like.h>
#include <sol/force_inline.h>
#include <sol/lbind/convert_argument.h>
#include <sol/lbind/convert_result.h>
#include <sol/lbind/semantic_wrapper.h>

namespace sol::lbind::detail {
    constexpr inline int RETURNS_STACK_MARGIN = 5;

    template <class Ret>
    struct ProcessReturns;

    template <>
    struct ProcessReturns<void> {
        template <auto F, class... Args> SOL_FORCE_INLINE
        static int finalInvoke(lua_State*, Args&&... args) {
            std::invoke(F, std::forward<Args>(args)...);
            return 0;
        }
    };

    template <TupleLike T>
    struct ProcessReturns<Values<T>> {
        template <auto F, class... Args> SOL_FORCE_INLINE
        static int finalInvoke(lua_State* state, Args&&... args) {
            std::apply(
                [&](auto&&... values) {
                    luaL_checkstack(state, sizeof...(values) + RETURNS_STACK_MARGIN, nullptr);
                    (..., ConvertResult<decltype(values)>::convert(state, std::forward<decltype(values)>(values)));
                },
                *std::invoke(F, std::forward<Args>(args)...)
            );

            return std::tuple_size_v<T>;
        }
    };

    template <ResultConvertible T>
    struct ProcessReturns<T> {
        template <auto F, class... Args> SOL_FORCE_INLINE
        static int finalInvoke(lua_State* state, Args&&... args) {
            auto&& ret = std::invoke(F, std::forward<Args>(args)...);
            luaL_checkstack(state, 1 + RETURNS_STACK_MARGIN, nullptr);
            ConvertResult<T>::convert(state, std::forward<decltype(ret)>(ret));
            return 1;
        }
    };

    template <int I, auto F, class R, class... Passed, class ToPass, class... MoreToPass> SOL_FORCE_INLINE
    static int passArgs(lua_State* state, Passed&&... passed) {
        return ::sol::lbind::detail::passArgs<I + 1, F, R, MoreToPass...>(
            state,
            std::forward<Passed>(passed)...,
            ConvertArgument<ToPass>::convert(state, I)
        );
    }

    template <int, auto F, class R, class... Passed> SOL_FORCE_INLINE
    static int passArgs(lua_State* state, Passed&&... passed) {
        return ProcessReturns<R>::template finalInvoke<F>(state, std::forward<Passed>(passed)...);
    }

    template <auto F, class R, class... Args> SOL_FORCE_INLINE
    static int doInvoke(lua_State* state) {
        return ::sol::lbind::detail::passArgs<1, F, R, Args...>(state);
    }

    template <auto F>
    struct WrapperFunction;

    template <class Ret, ArgumentConvertible... Args, Ret(*F)(Args...)>
    struct WrapperFunction<F> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return ::sol::lbind::detail::doInvoke<F, Ret, Args...>(state);
        }
    };

    template <class Ret, class T, ArgumentConvertible... Args, Ret (T::*F)(Args...)>
    requires ArgumentConvertible<T&>
    struct WrapperFunction<F> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return ::sol::lbind::detail::doInvoke<F, Ret, T&, Args...>(state);
        }
    };

    template <class Ret, class T, ArgumentConvertible... Args, Ret (T::*F)(Args...) const>
    requires ArgumentConvertible<T const&>
    struct WrapperFunction<F> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return ::sol::lbind::detail::doInvoke<F, Ret, T const&, Args...>(state);
        }
    };

    template <auto F, auto M>
    struct FunctionObjectWrapperFunction;

    template <class Ret, ArgumentConvertible... Args, auto F, Ret (decltype(F)::*M)(Args...) const>
    struct FunctionObjectWrapperFunction<F, M> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return ::sol::lbind::detail::doInvoke<F, Ret, Args...>(state);
        }
    };

    template <HasUnambiguousCallOperator auto F>
    struct WrapperFunction<F>: FunctionObjectWrapperFunction<F, &decltype(F)::operator()> {};
}
