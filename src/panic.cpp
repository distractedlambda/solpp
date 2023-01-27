#include <sol/panic.h>

#include <cstdlib>

void sol::panic() {
    std::abort();
}
