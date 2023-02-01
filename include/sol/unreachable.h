#pragma once

#include <sol/force_inline.h>

namespace sol {
    [[noreturn, gnu::cold]] SOL_FORCE_INLINE
    void unreachable() {
        __builtin_unreachable();
    }
}
