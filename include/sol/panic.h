#pragma once

#include <sol/force_inline.h>

namespace sol {
    [[noreturn, gnu::cold]] SOL_FORCE_INLINE
    inline void panic() {
        __builtin_trap();
    }
}
