#include <cstdint>
#include "uint128_t.h"
#include "gf.h"
#include "tables.h"
#include "aes.h"

uint32_t sub_bytes32(uint32_t val)
{
    uint32_t res = 0;
    for (int i = 0; i < 4; i++)
    {
        uint8_t byte = (val >> (8 * i)) & 0xFF;
        byte = MATRIX[byte];
        res |= byte << (8 * i);
    }
    return val;
}

uint128_t sub_bytes128(uint128_t val)
{
    for (int i = 0; i < 16; i++)
    {
        uint8_t byte = val.get_byte(i);
        byte = MATRIX[byte];
        val.set_byte(i, byte);
    }
    return val;
}

uint32_t inv_sub_bytes32(uint32_t val)
{
    uint32_t res = 0;
    for (int i = 0; i < 4; i++)
    {
        uint8_t byte = (val >> (8 * i)) & 0xFF;
        byte = MATRIX_INV[byte];
        res |= byte << (8 * i);
    }
    return val;
}

uint128_t inv_sub_bytes128(uint128_t val)
{
    for (int i = 0; i < 16; i++)
    {
        uint8_t byte = val.get_byte(i);
        byte = MATRIX_INV[byte];
        val.set_byte(i, byte);
    }
    return val;
}

uint128_t shift_rows(uint128_t val)
{
    uint128_t res;
    res.set_byte(0, val.get_byte(0));
    res.set_byte(4, val.get_byte(4));
    res.set_byte(8, val.get_byte(8));
    res.set_byte(12, val.get_byte(12));
    res.set_byte(0 + 1, val.get_byte(4 + 1));
    res.set_byte(4 + 1, val.get_byte(8 + 1));
    res.set_byte(8 + 1, val.get_byte(12 + 1));
    res.set_byte(12 + 1, val.get_byte(0 + 1));
    res.set_byte(0 + 2, val.get_byte(8 + 2));
    res.set_byte(4 + 2, val.get_byte(12 + 2));
    res.set_byte(8 + 2, val.get_byte(0 + 2));
    res.set_byte(12 + 2, val.get_byte(4 + 2));
    res.set_byte(0 + 3, val.get_byte(12 + 3));
    res.set_byte(4 + 3, val.get_byte(0 + 3));
    res.set_byte(8 + 3, val.get_byte(4 + 3));
    res.set_byte(12 + 3, val.get_byte(8 + 3));
    return res;
}

uint128_t inv_shift_rows(uint128_t val)
{
    uint128_t res;
    res.set_byte(0, val.get_byte(0));
    res.set_byte(4, val.get_byte(4));
    res.set_byte(8, val.get_byte(8));
    res.set_byte(12, val.get_byte(12));
    res.set_byte(0 + 1, val.get_byte(12 + 1));
    res.set_byte(4 + 1, val.get_byte(0 + 1));
    res.set_byte(8 + 1, val.get_byte(4 + 1));
    res.set_byte(12 + 1, val.get_byte(8 + 1));
    res.set_byte(0 + 2, val.get_byte(8 + 2));
    res.set_byte(4 + 2, val.get_byte(12 + 2));
    res.set_byte(8 + 2, val.get_byte(0 + 2));
    res.set_byte(12 + 2, val.get_byte(4 + 2));
    res.set_byte(0 + 3, val.get_byte(4 + 3));
    res.set_byte(4 + 3, val.get_byte(8 + 3));
    res.set_byte(8 + 3, val.get_byte(12 + 3));
    res.set_byte(12 + 3, val.get_byte(0 + 3));
    return res;
}

uint128_t mix_columns(uint128_t val)
{
    for (int i = 0; i < 4; i++)
    {
        uint32_t word = val.get_word(i);
        uint32_t res = 0;
        res |= gdot(word, 0x01010302) << (0 * 8);
        res |= gdot(word, 0x01030201) << (1 * 8);
        res |= gdot(word, 0x03020101) << (2 * 8);
        res |= gdot(word, 0x02010103) << (3 * 8);
        val.set_word(i, res);
    }
    return val;
}

uint128_t inv_mix_columns(uint128_t val)
{
    for (int i = 0; i < 4; i++)
    {
        uint32_t word = val.get_word(i);
        uint32_t res = 0;
        res |= gdot(word, 0x090D0B0E) << (0 * 8);
        res |= gdot(word, 0x0D0B0E09) << (1 * 8);
        res |= gdot(word, 0x0B0E090D) << (2 * 8);
        res |= gdot(word, 0x0E090D0B) << (3 * 8);
        val.set_word(i, res);
    }
    return val;
}

uint32_t schedule_core(uint32_t val, int i)
{
    // rotate right 8 bits
    val = (val >> 8) | (val << (32 - 8));
    val = sub_bytes32(val);
    uint8_t first_byte = val & 0xFF;
    first_byte ^= RCON[i];
    val = (val & ~0xFF) | first_byte;
    return val;
}

std::array<uint128_t, 11> expand_key(uint128_t key)
{
    std::array<uint128_t, 11> w;
    w[0] = key;
    for (int i = 0; i < 10; i++)
    {
        uint32_t w_prev_0 = w[i].get_word(0);
        uint32_t w_prev_1 = w[i].get_word(1);
        uint32_t w_prev_2 = w[i].get_word(2);
        uint32_t w_prev_3 = w[i].get_word(3);
        uint32_t w_new_0 = w_prev_0 ^ schedule_core(w_prev_3, i);
        uint32_t w_new_1 = w_prev_1 ^ w_new_0;
        uint32_t w_new_2 = w_prev_2 ^ w_new_1;
        uint32_t w_new_3 = w_prev_3 ^ w_new_2;
        w[i + 1]
            .set_word(0, w_new_0)
            .set_word(1, w_new_1)
            .set_word(2, w_new_2)
            .set_word(3, w_new_3);
    }
    return w;
}

uint128_t cipher_block(const std::array<uint128_t, 11> &keys, uint128_t block)
{
    block ^= keys[0];
    for (int i = 1; i <= 9; i++)
    {
        block = sub_bytes128(block);
        block = shift_rows(block);
        block = mix_columns(block);
        block ^= keys[i];
    }
    block = sub_bytes128(block);
    block = shift_rows(block);
    block ^= keys[10];
    return block;
}

uint128_t decipher_block(const std::array<uint128_t, 11> &keys, uint128_t block)
{
    block ^= keys[10];
    block = inv_shift_rows(block);
    block = inv_sub_bytes128(block);
    for (int i = 9; i >= 1; i--)
    {
        block ^= keys[i];
        block = inv_mix_columns(block);
        block = inv_shift_rows(block);
        block = inv_sub_bytes128(block);
    }
    block ^= keys[0];
    return block;
}

uint128_t AESCryptor::encrypt(uint128_t data)
{
    auto keys = expand_key(key);
    return cipher_block(keys, data);
}

uint128_t AESCryptor::decrypt(uint128_t data)
{
    auto keys = expand_key(key);
    return decipher_block(keys, data);
}
