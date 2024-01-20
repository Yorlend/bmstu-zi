#pragma once

#include <cstdint>

static uint8_t gmul_byte(uint8_t a, uint8_t b)
{
    uint8_t res = 0;
    for (int i = 0; i < 8; i++)
    {
        if (b & 1)
        {
            res ^= a;
        }
        uint8_t hi_bit_set = a & 0x80;
        a <<= 1;
        if (hi_bit_set == 0x80)
        {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return res;
}

static uint8_t gdot(uint32_t a, uint32_t b)
{
    uint8_t res = 0;
    res ^= gmul_byte((a >> 0) & 0xFF, (b >> 0) & 0xFF);
    res ^= gmul_byte((a >> 8) & 0xFF, (b >> 8) & 0xFF);
    res ^= gmul_byte((a >> 16) & 0xFF, (b >> 16) & 0xFF);
    res ^= gmul_byte((a >> 24) & 0xFF, (b >> 24) & 0xFF);
    return res;
}
