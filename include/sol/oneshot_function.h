#pragma once

#include <sol/guard.h>

#include <memory>

namespace sol {
    template <class F>
    class OneshotFunction;

    template <class Ret, class... Args>
    class OneshotFunction<Ret(Args...)> {
    public:
        SOL_FORCE_INLINE
        OneshotFunction(OneshotFunction&&) noexcept = default;

        template <class F> SOL_FORCE_INLINE
        OneshotFunction(std::in_place_type_t<F>, auto&&... args): _impl(
            new Impl<F>(std::forward<decltype(args)>(args)...)
        ) {}

        SOL_FORCE_INLINE
        OneshotFunction(auto&& arg): _impl(
            new Impl<std::decay_t<decltype(arg)>>(std::forward<decltype(arg)>(arg))
        ) {}

        SOL_FORCE_INLINE
        ~OneshotFunction() = default;

        SOL_FORCE_INLINE
        OneshotFunction& operator=(OneshotFunction&&) noexcept = default;

        SOL_FORCE_INLINE
        Ret operator()(Args... args) {
            auto* impl = _impl.release();
            sol::Guard deleteImpl = [&] { delete impl; };
            return impl->operator()(std::forward<Args>(args)...);
        }

    private:
        class ImplBase {
        public:
            virtual Ret operator()(Args...) = 0;

            virtual ~ImplBase() = default;
        };

        std::unique_ptr<ImplBase> _impl;

        template <class F>
        class Impl final: public ImplBase {
        public:
            SOL_FORCE_INLINE
            Impl(auto&&... args): _wrapped(std::forward<decltype(args)>(args)...) {}

            Ret operator()(Args... args) final {
                return std::invoke(std::move(_wrapped), std::forward<Args>(args)...);
            }

            ~Impl() final = default;

        private:
            F _wrapped;
        };
    };
}
