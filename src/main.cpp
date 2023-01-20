#include <sol/lbind.h>

#include <cmath>

#include <iostream>

int main() {
    auto f = sol::lbind::wrappedFunction<powf>;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
