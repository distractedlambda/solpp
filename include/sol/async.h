#pragma once

#include <sol/oneshot_function.h>

namespace sol {
    void async(OneshotFunction<void()> job);
}
