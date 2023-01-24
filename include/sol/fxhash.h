#pragma once

#include <bit>
#include <string_view>

namespace sol::detail {
    constexpr inline uint64_t FXHASH_SEED = UINT64_C(0x51'7c'c1'b7'27'22'0a'95);

    constexpr inline unsigned int FXHASH_ROTATE = 5;

    constexpr void fxhashWord(uint64_t& hash, uint64_t word) {
        hash = (std::rotl(hash, FXHASH_ROTATE) ^ word) * FXHASH_SEED;
    }
}

namespace sol {
    constexpr uint64_t fxhash(std::string_view input) {
        uint64_t hash = 0;

        while (input.size() >= 8) {
            uint64_t word = uint64_t(uint8_t(input[0]))
                | (uint64_t(uint8_t(input[1])) << 8)
                | (uint64_t(uint8_t(input[2])) << 16)
                | (uint64_t(uint8_t(input[3])) << 24)
                | (uint64_t(uint8_t(input[4])) << 32)
                | (uint64_t(uint8_t(input[5])) << 40)
                | (uint64_t(uint8_t(input[6])) << 48)
                | (uint64_t(uint8_t(input[7])) << 56);

            ::sol::detail::fxhashWord(hash, word);

            input = input.substr(8);
        }

        if (input.size() >= 4) {
            uint64_t word = uint64_t(uint8_t(input[0]))
                | (uint64_t(uint8_t(input[1])) << 8)
                | (uint64_t(uint8_t(input[2])) << 16)
                | (uint64_t(uint8_t(input[3])) << 24);

            ::sol::detail::fxhashWord(hash, word);

            input = input.substr(4);
        }

        if (input.size() >= 2) {
            uint64_t word = uint64_t(uint8_t(input[0]))
                | (uint64_t(uint8_t(input[1])) << 8);

            ::sol::detail::fxhashWord(hash, word);

            input = input.substr(2);
        }

        if (!input.empty()) {
            ::sol::detail::fxhashWord(hash, uint8_t(input[0]));
        }

        return hash;
    }
}
