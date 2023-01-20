#include <sol/force_inline.h>

#include <concepts>

namespace sol {
    template <std::invocable<> F>
    class Guard {
    public:
        template <class Arg> SOL_FORCE_INLINE
        Guard(Arg&& arg): _action(std::forward<Arg>(arg)) {}

        Guard(Guard const&) = delete;

        auto operator=(Guard const&) -> Guard& = delete;

        SOL_FORCE_INLINE
        ~Guard() {
            std::invoke(std::move(_action));
        }

    private:
        F _action;
    };

    template <class Arg>
    Guard(Arg&& arg) -> Guard<std::decay_t<Arg>>;
}
