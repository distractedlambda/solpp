#include <sol/glfw/error.h>

#include <GLFW/glfw3.h>

#include <stdexcept>

void sol::glfw::checkError() {
    char const* description;
    if (glfwGetError(&description)) {
        throw std::runtime_error(description);
    }
}
