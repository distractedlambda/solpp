#include <sol/log.h>

void sol::log::detail::print(Severity severity, fmt::string_view format, fmt::format_args args) {
    auto severityToken = [&]{
        switch (severity) {
            case Panic:
                return 'P';
            case Error:
                return 'E';
            case Warning:
                return 'W';
            case Info:
                return 'I';
            case Debug:
                return 'D';
            case Trace:
                return 'T';
        }
    }();

    fmt::print(stderr, "[{}] {}\n", severityToken, fmt::vformat(format, args));
}
