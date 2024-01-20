#pragma once

#include <cstdint>
#include <cstddef>
#include <utility>

class uint128_t
{
public:
    uint128_t() : words{0, 0, 0, 0} {}

    uint8_t get_byte(uint8_t index) const
    {
        return bytes[index];
    }

    uint128_t &set_byte(uint8_t index, uint8_t byte)
    {
        bytes[index] = byte;
        return *this;
    }

    uint32_t get_word(uint8_t index)
    {
        return words[index];
    }

    uint128_t &set_word(uint8_t index, uint32_t word)
    {
        words[index] = word;
        return *this;
    }

    uint128_t &reverse_bytes()
    {
        for (uint8_t i = 0; i < 8; i++)
            std::swap(bytes[i], bytes[15 - i]);
        return *this;
    }

    uint128_t operator^(const uint128_t &other)
    {
        uint128_t res;
        for (int i = 0; i < 4; i++)
            res.words[i] = words[i] ^ other.words[i];
        return res;
    }

    uint128_t &operator^=(const uint128_t &other)
    {
        for (int i = 0; i < 4; i++)
            words[i] ^= other.words[i];
        return *this;
    }

    static uint128_t pack_bytes(const uint8_t (&bytes)[16])
    {
        uint128_t result;
        for (size_t i = 0; i < 16; i++)
            result.bytes[i] = bytes[i];
        return result;
    }

    union
    {
        uint8_t bytes[16];
        uint32_t words[4];
    };
};
