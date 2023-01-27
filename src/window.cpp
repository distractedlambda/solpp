#include <sol/glfw/error.h>
#include <sol/overload.h>
#include <sol/panic.h>
#include <sol/window.h>

#include <GLFW/glfw3.h>

void sol::Window::Handler::position(Eigen::Vector2i position) {}

void sol::Window::Handler::size(Eigen::Vector2i size) {}

void sol::Window::Handler::closeRequest() {}

void sol::Window::Handler::focus(bool focused) {}

void sol::Window::Handler::iconification(bool iconified) {}

void sol::Window::Handler::maximization(bool maximized) {}

void sol::Window::Handler::contentScale(Eigen::Vector2f scale) {}

void sol::Window::Handler::mouseButton(int button, int action, int modifiers) {}

void sol::Window::Handler::cursorPosition(Eigen::Vector2d position) {}

void sol::Window::Handler::cursorEntry(bool entered) {}

void sol::Window::Handler::scroll(Eigen::Vector2d offset) {}

void sol::Window::Handler::key(int key, int scancode, int action, int modifiers) {}

void sol::Window::Handler::character(uint32_t codepoint) {}

void sol::Window::Handler::drop(std::span<const char* const> paths) {}

sol::Window::Handler::~Handler() = default;

sol::Window::Window(int width, int height, const char* title, std::span<const Option> options) {
    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    for (auto& option : options) {
        std::visit(overload(
            [](Resizable const& option) { glfwWindowHint(GLFW_RESIZABLE, option.value); },
            [](Visible const& option) { glfwWindowHint(GLFW_VISIBLE, option.value); },
            [](Decorated const& option) { glfwWindowHint(GLFW_DECORATED, option.value); },
            [](Focused const& option) { glfwWindowHint(GLFW_FOCUSED, option.value); },
            [](Maximized const& option) { glfwWindowHint(GLFW_MAXIMIZED, option.value); },
            [](FocusOnShow const& option) { glfwWindowHint(GLFW_FOCUS_ON_SHOW, option.value); },
            [](ScaleToMonitor const& option) { glfwWindowHint(GLFW_SCALE_TO_MONITOR, option.value); }
        ), option);
    }

    _glfwWindow.reset(glfwCreateWindow(width, height, title, nullptr, nullptr), [](GLFWwindow* window) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            delete static_cast<Handler*>(handler);
        }
    });

    if (!_glfwWindow) {
        glfw::checkError();
        panic();
    }

    glfwSetWindowPosCallback(_glfwWindow.get(), [](GLFWwindow* window, int x, int y) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->position({x, y});
        }
    });

    glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, int width, int height) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->size({width, height});
        }
    });

    glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->closeRequest();
        }
    });

    glfwSetWindowFocusCallback(_glfwWindow.get(), [](GLFWwindow* window, int focused) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->focus(focused);
        }
    });

    glfwSetWindowIconifyCallback(_glfwWindow.get(), [](GLFWwindow* window, int iconified) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->iconification(iconified);
        }
    });

    glfwSetWindowMaximizeCallback(_glfwWindow.get(), [](GLFWwindow* window, int maximized) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->maximization(maximized);
        }
    });

    glfwSetWindowContentScaleCallback(_glfwWindow.get(), [](GLFWwindow* window, float xScale, float yScale) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->contentScale({xScale, yScale});
        }
    });

    glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, int button, int action, int modifiers) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->mouseButton(button, action, modifiers);
        }
    });

    glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, double x, double y) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->cursorPosition({x, y});
        }
    });

    glfwSetCursorEnterCallback(_glfwWindow.get(), [](GLFWwindow* window, int entered) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->cursorEntry(entered);
        }
    });

    glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, double x, double y) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->scroll({x, y});
        }
    });

    glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, int key, int scancode, int action, int modifiers) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->key(key, scancode, action, modifiers);
        }
    });

    glfwSetCharCallback(_glfwWindow.get(), [](GLFWwindow* window, unsigned int codepoint) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->character(codepoint);
        }
    });

    glfwSetDropCallback(_glfwWindow.get(), [](GLFWwindow* window, int pathCount, char const* paths[]) {
        if (auto handler = glfwGetWindowUserPointer(window)) {
            static_cast<Handler*>(handler)->drop({paths, size_t(pathCount)});
        }
    });
}

std::unique_ptr<sol::Window::Handler> sol::Window::setHandler(std::unique_ptr<Handler> newHandler) {
    auto oldHandler = glfwGetWindowUserPointer(_glfwWindow.get());
    glfwSetWindowUserPointer(_glfwWindow.get(), newHandler.release());
    return std::unique_ptr<Handler>(static_cast<Handler*>(oldHandler));
}

Eigen::Vector2i sol::Window::position() const {
    int x, y;
    glfwGetWindowPos(_glfwWindow.get(), &x, &y);
    glfw::checkError();
    return {x, y};
}

Eigen::Vector2i sol::Window::size() const {
    int w, h;
    glfwGetWindowSize(_glfwWindow.get(), &w, &h);
    glfw::checkError();
    return {w, h};
}

Eigen::Vector2f sol::Window::contentScale() const {
    float x, y;
    glfwGetWindowContentScale(_glfwWindow.get(), &x, &y);
    glfw::checkError();
    return {x, y};
}

float sol::Window::opacity() const {
    auto opacity = glfwGetWindowOpacity(_glfwWindow.get());
    glfw::checkError();
    return opacity;
}

void sol::Window::setTitle(char const* title) {
    glfwSetWindowTitle(_glfwWindow.get(), title);
    glfw::checkError();
}

void sol::Window::setPosition(Eigen::Vector2i position) {
    glfwSetWindowPos(_glfwWindow.get(), position.x(), position.y());
    glfw::checkError();
}

void sol::Window::setSize(Eigen::Vector2i size) {
    glfwSetWindowSize(_glfwWindow.get(), size.x(), size.y());
    glfw::checkError();
}

void sol::Window::setSizeLimits(Eigen::Vector2i min, Eigen::Vector2i max) {
    glfwSetWindowSizeLimits(_glfwWindow.get(), min.x(), min.y(), max.x(), max.y());
    glfw::checkError();
}

void sol::Window::setAspectRatio(int numerator, int denominator) {
    glfwSetWindowAspectRatio(_glfwWindow.get(), numerator, denominator);
    glfw::checkError();
}

void sol::Window::setOpacity(float opacity) {
    glfwSetWindowOpacity(_glfwWindow.get(), opacity);
    glfw::checkError();
}

void sol::Window::iconify() {
    glfwIconifyWindow(_glfwWindow.get());
    glfw::checkError();
}

void sol::Window::restore() {
    glfwRestoreWindow(_glfwWindow.get());
    glfw::checkError();
}

void sol::Window::maximize() {
    glfwMaximizeWindow(_glfwWindow.get());
    glfw::checkError();
}

void sol::Window::show() {
    glfwShowWindow(_glfwWindow.get());
    glfw::checkError();
}

void sol::Window::hide() {
    glfwHideWindow(_glfwWindow.get());
    glfw::checkError();
}

void sol::Window::focus() {
    glfwFocusWindow(_glfwWindow.get());
    glfw::checkError();
}

void sol::Window::requestAttention() {
    glfwRequestWindowAttention(_glfwWindow.get());
    glfw::checkError();
}
