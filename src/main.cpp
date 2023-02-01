#include <sol/glfw/error.h>
#include <sol/guard.h>
#include <sol/log.h>
#include <sol/panic.h>

#include <GLFW/glfw3.h>

namespace sol {
    static int protectedMain(int argc, char** argv) {
        if (!glfwInit()) {
            glfw::checkError();
            panic();
        }

        Guard terminateGlfw = glfwTerminate;

        for (;;) {
            glfwWaitEvents();
        }

        return EXIT_SUCCESS;
    }

    static int main(int argc, char** argv) {
        try {
            return protectedMain(argc, argv);
        } catch (std::exception const& exception) {
            log::panic("Fatal exception in main thread: {}", exception.what());
            return EXIT_FAILURE;
        }
    }
}

int main(int argc, char** argv) {
    return sol::main(argc, argv);
}
