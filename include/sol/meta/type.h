#pragma once

namespace sol {
    template <class T>
    struct Type {
        using type = T;
    };

    template <class T>
    constexpr inline Type<T> type;
}
