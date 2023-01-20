#pragma once

#include <sol/force_inline.h>

#include <memory>

namespace sol::lbind::detail {
    template <class T, class>
    class SemanticWrapper {
    public:
        SemanticWrapper() = delete;

        SOL_FORCE_INLINE
        SemanticWrapper(T const& wrapped): _wrapped(wrapped) {}

        SOL_FORCE_INLINE
        SemanticWrapper(T&& wrapped): _wrapped(std::forward<T>(wrapped)) {}

        template <class... Args> SOL_FORCE_INLINE
        SemanticWrapper(std::in_place_t, Args&&... args): _wrapped(std::forward<Args>(args)...) {}

        SOL_FORCE_INLINE
        SemanticWrapper(SemanticWrapper const&) = default;

        SOL_FORCE_INLINE
        SemanticWrapper(SemanticWrapper&&) noexcept = default;

        SOL_FORCE_INLINE
        SemanticWrapper& operator=(SemanticWrapper const&) = default;

        SOL_FORCE_INLINE
        SemanticWrapper& operator=(SemanticWrapper&&) noexcept = default;

        SOL_FORCE_INLINE
        ~SemanticWrapper() = default;

        [[nodiscard]] SOL_FORCE_INLINE
        T const& operator*() const& {
            return _wrapped;
        }

        [[nodiscard]] SOL_FORCE_INLINE
        T& operator*() & {
            return _wrapped;
        }

        [[nodiscard]] SOL_FORCE_INLINE
        T&& operator*() && {
            return std::move(_wrapped);
        }

        [[nodiscard]] SOL_FORCE_INLINE
        T const* operator->() const {
            return std::addressof(_wrapped);
        }

        [[nodiscard]] SOL_FORCE_INLINE
        T* operator->() {
            return std::addressof(_wrapped);
        }

    private:
        T _wrapped;
    };

    struct ValuesTag {};
}

namespace sol::lbind {
    template <class T>
    using Values = detail::SemanticWrapper<T, detail::ValuesTag>;
}
