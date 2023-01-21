#include <sol/lbind.h>

#include <iostream>

int main() {
    auto f = sol::lbind::wrappedFunction<[] { return 5; }>;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
