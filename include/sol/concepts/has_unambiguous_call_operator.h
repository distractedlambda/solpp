#pragma once

namespace sol {
    template <class Self>
    concept HasUnambiguousCallOperator = requires {
        &Self::operator();
    };
}
