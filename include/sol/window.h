#pragma once

#include <Eigen/Core>

#include <memory>
#include <span>
#include <variant>

struct GLFWwindow;

namespace sol {
    class Window {
    public:
        struct Resizable {
            bool value = true;
        };

        struct Visible {
            bool value = true;
        };

        struct Decorated {
            bool value = true;
        };

        struct Focused {
            bool value = true;
        };

        struct Maximized {
            bool value = true;
        };

        struct FocusOnShow {
            bool value = true;
        };

        struct ScaleToMonitor {
            bool value = true;
        };

        using Option = std::variant<
            Resizable,
            Visible,
            Decorated,
            Focused,
            Maximized,
            FocusOnShow,
            ScaleToMonitor
        >;

        class Handler {
        public:
            virtual void position(Eigen::Vector2i position);

            virtual void size(Eigen::Vector2i size);

            virtual void closeRequest();

            virtual void focus(bool focused);

            virtual void iconification(bool iconified);

            virtual void maximization(bool maximized);

            virtual void contentScale(Eigen::Vector2f scale);

            virtual void mouseButton(int button, int action, int modifiers);

            virtual void cursorPosition(Eigen::Vector2d position);

            virtual void cursorEntry(bool entered);

            virtual void scroll(Eigen::Vector2d offset);

            virtual void key(int key, int scancode, int action, int modifiers);

            virtual void character(uint32_t codepoint);

            virtual void drop(std::span<char const* const> paths);

            virtual ~Handler();
        };

        Window(int window, int height, char const* title, std::span<Option const> options = {});

        std::unique_ptr<Handler> setHandler(std::unique_ptr<Handler> newHandler);

        Eigen::Vector2i position() const;

        Eigen::Vector2i size() const;

        Eigen::Vector2f contentScale() const;

        float opacity() const;

        void setTitle(char const* title);

        void setPosition(Eigen::Vector2i position);

        void setSize(Eigen::Vector2i size);

        void setSizeLimits(Eigen::Vector2i min, Eigen::Vector2i max);

        void setAspectRatio(int numerator, int denominator);

        void setOpacity(float opacity);

        void iconify();

        void restore();

        void maximize();

        void show();

        void hide();

        void focus();

        void requestAttention();

    private:
        std::shared_ptr<GLFWwindow> _glfwWindow;
    };
}
