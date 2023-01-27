#include <sol/window.h>

#include <iostream>

int main() {
    sol::Window window(640, 480, "Hi", {{sol::Window::Resizable{false}, sol::Window::Visible{true}}});
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
