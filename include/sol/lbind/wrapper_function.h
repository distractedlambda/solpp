#pragma once

#include <sol/concepts/has_unambiguous_const_call_operator.h>
#include <sol/concepts/tuple_like.h>
#include <sol/force_inline.h>
#include <sol/lbind/convert_argument.h>
#include <sol/lbind/convert_result.h>
#include <sol/lbind/semantic_wrapper.h>

namespace sol::lbind::detail {
    constexpr inline int RETURNS_STACK_MARGIN = 5;

    template <auto F, class Ret, int I, class... Args>
    struct WrapperFunctionImpl;

    template <auto F, class Ret, int I, class Arg, class... MoreArgs>
    struct WrapperFunctionImpl<F, Ret, I, Arg, MoreArgs...> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state, auto&&... passed) {
            return WrapperFunctionImpl<F, Ret, I + 1, MoreArgs...>::invoke(
                state,
                std::forward<decltype(passed)>(passed)...,
                ConvertArgument<Arg>::convert(state, I)
            );
        }
    };

    template <auto F, int I>
    struct WrapperFunctionImpl<F, void, I> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state, auto&&... passed) {
            std::invoke(F, std::forward<decltype(passed)>(passed)...);
            return 0;
        }
    };

    template <auto F, int I, TupleLike T>
    struct WrapperFunctionImpl<F, Values<T>, I> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state, auto&&... passed) {
            std::apply(
                [&](auto&&... values) {
                    luaL_checkstack(state, sizeof...(values) + RETURNS_STACK_MARGIN, nullptr);
                    (..., ConvertResult<decltype(values)>::convert(state, std::forward<decltype(values)>(values)));
                },
                *std::invoke(F, std::forward<decltype(passed)>(passed)...)
            );

            return std::tuple_size_v<T>;
        }
    };

    template <auto F, int I, ResultConvertible Ret>
    struct WrapperFunctionImpl<F, Ret, I> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state, auto&&... args) {
            auto&& ret = std::invoke(F, std::forward<decltype(args)>(args)...);
            luaL_checkstack(state, 1 + RETURNS_STACK_MARGIN, nullptr);
            ConvertResult<Ret>::convert(state, std::forward<decltype(ret)>(ret));
            return 1;
        }
    };

    template <auto F>
    struct WrapperFunction;

    template <class Ret, ArgumentConvertible... Args, Ret(*F)(Args...)>
    struct WrapperFunction<F> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return WrapperFunctionImpl<F, Ret, 1, Args...>::invoke(state);
        }
    };

    template <class Ret, class T, ArgumentConvertible... Args, Ret (T::*F)(Args...)>
    requires ArgumentConvertible<T&>
    struct WrapperFunction<F> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return WrapperFunctionImpl<F, Ret, 1, T&, Args...>::invoke(state);
        }
    };

    template <class Ret, class T, ArgumentConvertible... Args, Ret (T::*F)(Args...) const>
    requires ArgumentConvertible<T const&>
    struct WrapperFunction<F> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return WrapperFunctionImpl<F, Ret, 1, T const&, Args...>::invoke(state);
        }
    };

    template <auto F, auto M>
    struct FunctionObjectWrapperFunction;

    template <class Ret, ArgumentConvertible... Args, auto F, Ret (decltype(F)::*M)(Args...) const>
    struct FunctionObjectWrapperFunction<F, M> {
        SOL_FORCE_INLINE
        static int invoke(lua_State* state) {
            return WrapperFunctionImpl<F, Ret, 1, Args...>::invoke(state);
        }
    };

    template <HasUnambiguousConstCallOperator auto F>
    struct WrapperFunction<F>: FunctionObjectWrapperFunction<F, &decltype(F)::operator()> {};

    template <auto F>
    constexpr inline lua_CFunction wrappedFunction = &detail::WrapperFunction<F>::invoke;
}
