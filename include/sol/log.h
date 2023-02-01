#pragma once

#include <sol/force_inline.h>

#include <fmt/core.h>

namespace sol::log {
    enum Severity {
        Panic,
        Error,
        Warning,
        Info,
        Debug,
        Trace,
    };
}

namespace sol::log::detail {
    void print(Severity severity, fmt::string_view format, fmt::format_args args);
}

namespace sol::log {
    template <class... Args> SOL_FORCE_INLINE
    void print(Severity severity, fmt::format_string<Args...> format, Args&&... args) {
        ::sol::log::detail::print(severity, format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    template <class... Args> SOL_FORCE_INLINE
    void panic(fmt::format_string<Args...> format, Args&&... args) {
        ::sol::log::detail::print(Panic, format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    template <class... Args> SOL_FORCE_INLINE
    void error(fmt::format_string<Args...> format, Args&&... args) {
        ::sol::log::detail::print(Error, format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    template <class... Args> SOL_FORCE_INLINE
    void warning(fmt::format_string<Args...> format, Args&&... args) {
        ::sol::log::detail::print(Warning, format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    template <class... Args> SOL_FORCE_INLINE
    void info(fmt::format_string<Args...> format, Args&&... args) {
        ::sol::log::detail::print(Info, format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    template <class... Args> SOL_FORCE_INLINE
    void debug(fmt::format_string<Args...> format, Args&&... args) {
        ::sol::log::detail::print(Debug, format, fmt::make_format_args(std::forward<Args>(args)...));
    }

    template <class... Args> SOL_FORCE_INLINE
    void trace(fmt::format_string<Args...> format, Args&&... args) {
        ::sol::log::detail::print(Trace, format, fmt::make_format_args(std::forward<Args>(args)...));
    }
}
