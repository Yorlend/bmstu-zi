#include <array>
#include <bitset>
#include <cstring>
#include "des.h"

static int __C0[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, // D0
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

static int __Si[] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

static int __CP[] = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

static int __IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

static int __E[] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

static int __S[][64] = {
    {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
    },
    {
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
    },
    {
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
    },
    {
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
    },
    {
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
    },
    {
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
    },
    {
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
    },
    {
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    }
};

static int __P[] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

static int __IPm1[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

template<size_t N>
std::string to_str(const std::bitset<N>& bits) {
    auto s = bits.to_string();
    return std::string(s.rbegin(), s.rend());
}

template<size_t N>
static unsigned long long to_ulong(const std::bitset<N>& bits)
{
    unsigned long long result = 0;
    for (size_t i = 0; i < N; ++i)
        result |= static_cast<unsigned long long>(bits[i]) << (N - i - 1);
    return result;
}

template<size_t M, size_t N>
static std::bitset<M> extend(const std::bitset<N>& bits, const int mask[], int offset = 0)
{
    std::bitset<M> result;
    for (size_t i = 0; i < M; ++i)
        result[i] = bits[mask[i] - offset];
    return result;
}

template<size_t N, size_t M>
static std::bitset<N + M> chain(const std::bitset<N>& left, const std::bitset<M>& right)
{
    std::bitset<N + M> result;
    for (size_t i = 0; i < N; ++i)
        result[i] = left[i];
    for (size_t i = 0; i < M; ++i)
        result[N + i] = right[i];
    return result;
}

template<size_t M, size_t N>
static std::bitset<M> subset(const std::bitset<N>& bits, int from = 0)
{
    std::bitset<M> result;
    for (size_t i = 0; i < M; ++i)
        result[i] = bits[i + from];
    return result;
}

template<size_t N>
static std::bitset<N> rotate_left(const std::bitset<N>& bits, int offset)
{
    std::bitset<N> result;
    for (size_t i = 0; i < N; ++i)
        result[i] = bits[(i + offset + N) % N];
    return result;
}

template<size_t N>
static std::bitset<N> permute(const std::bitset<N>& bits, const int p[], int offset = 0)
{
    std::bitset<N> result;
    for (size_t i = 0; i < N; i++)
        result[i] = bits[p[i] - offset];
    return result;
}

template<size_t N>
static std::bitset<N> from_ulong(unsigned long long val)
{
    std::bitset<N> result;
    for (size_t i = 0; i < N; ++i)
        result[i] = (val >> (N - i - 1)) & 1;
    return result;
}

static std::array<std::bitset<48>, 16> generate_round_keys(uint64_t base_key)
{
    auto base_bits = std::bitset<64>(base_key);
    auto zipped = extend<56>(base_bits, __C0, 1);

    auto C = subset<28>(zipped, 0);
    auto D = subset<28>(zipped, 28);

    std::array<std::bitset<48>, 16> round_keys;

    for (int i = 0; i < 16; ++i)
    {
        C = rotate_left(C, __Si[i]);
        D = rotate_left(D, __Si[i]);

        round_keys[i] = extend<48>(chain(C, D), __CP, 1);
    }

    return round_keys;
}

static std::bitset<32> pheistel(const std::bitset<32>& msg, const std::bitset<48>& key)
{
    auto ext_msg = extend<48>(msg, __E, 1) xor key;
    std::bitset<32> res = 0;

    for (size_t i = 0; i < 8; i++)
    {
        auto block = subset<6>(ext_msg, 6 * i);

        uint8_t row_id = ((uint8_t)(block[0]) << 1) | (uint8_t)block[5];
        uint8_t col_id = to_ulong(subset<4>(block, 1));

        auto vals = std::bitset<32>(__S[i][row_id * 16 + col_id]);

        for (size_t j = 0; j < 4; j++)
            res[4 * i + j] = vals[3 - j];
    }

    res = permute(res, __P, 1);
    return res;
}

static uint64_t encrypt_block(uint64_t block, const std::array<std::bitset<48>, 16>& round_keys)
{
    auto msg_bits = from_ulong<64>(block);
    msg_bits = permute(msg_bits, __IP, 1);

    auto left = subset<32>(msg_bits, 0);
    auto right = subset<32>(msg_bits, 32);

    for (int i = 0; i < 16; i++)
    {
        auto new_left = right;
        auto phe = pheistel(right, round_keys[i]);
        auto new_right = left xor phe;

        left = new_left;
        right = new_right;
    }

    msg_bits = chain(right, left);
    msg_bits = permute(msg_bits, __IPm1, 1);

    return to_ulong(msg_bits);
}

static uint64_t decrypt_block(uint64_t block, const std::array<std::bitset<48>, 16>& round_keys)
{
    auto msg_bits = from_ulong<64>(block);
    msg_bits = permute(msg_bits, __IP, 1);

    auto left = subset<32>(msg_bits, 0);
    auto right = subset<32>(msg_bits, 32);

    for (int i = 15; i >= 0; i--)
    {
        auto new_left = right;
        auto new_right = left xor pheistel(right, round_keys[i]);

        left = new_left;
        right = new_right;
    }

    msg_bits = chain(right, left);
    msg_bits = permute(msg_bits, __IPm1, 1);

    return to_ulong(msg_bits);
}

static void extend_block(uint8_t *buffer, size_t nread)
{
    buffer[7] = nread;
}

uint64_t buffer_to_block(const uint8_t *buffer)
{
    uint64_t block = 0;
    for (int i = 0; i < 8; i++)
        block |= static_cast<uint64_t>(buffer[i]) << (8 * (8 - i - 1));
    return block;
}

void block_to_buffer(uint64_t block, uint8_t *buffer)
{
    for (int i = 0; i < 8; i++)
        buffer[i] = (block >> (8 * (8 - i - 1))) & 0xFF;
}

DesCryptor::DesCryptor(uint64_t key) noexcept
    : key(key)
{}

void DesCryptor::encrypt(std::istream& input, std::ostream& output) const
{
    auto round_keys = generate_round_keys(key);
    bool run = true;
    bool first_pass = true;
    uint64_t prev_cyphered_block;
    do
    {
        uint8_t buffer[8];
        input.read(reinterpret_cast<char*>(buffer), 8);
        auto nread = input.gcount();
        if (nread < 8) {
            // extend to 64bit block
            extend_block(buffer, nread);
            run = false;
        }

        uint64_t block = buffer_to_block(buffer);
        if (first_pass)
            first_pass = false;
        else
            block = block xor prev_cyphered_block;
        block = encrypt_block(block, round_keys);
        prev_cyphered_block = block;
        block_to_buffer(block, buffer);

        output.write(reinterpret_cast<char*>(buffer), 8);
    } while (run);
}

void DesCryptor::decrypt(std::istream& input, std::ostream& output) const
{
    auto round_keys = generate_round_keys(key);
    uint64_t prev_block;
    uint8_t prev_buffer[8];
    uint8_t buffer[8];
    bool run = true;
    bool first_pass = true;
    do
    {
        input.read(reinterpret_cast<char*>(buffer), 8);
        auto nread = input.gcount();
        if (nread == 0)
            run = false;
        else if (nread < 8)
            throw std::runtime_error("invalid cyphered input size");

        uint64_t block = buffer_to_block(buffer);
        auto decyphered_block = decrypt_block(block, round_keys);
        if (!first_pass)
            decyphered_block = decyphered_block xor prev_block;
        prev_block = block;
        block_to_buffer(decyphered_block, buffer);

        // handle last block as extended
        size_t nwrite = run ? 8 : prev_buffer[7];

        if (first_pass)
            first_pass = false;
        else if (nwrite > 0)
            output.write(reinterpret_cast<char*>(prev_buffer), nwrite);
        memcpy(prev_buffer, buffer, 8);
    } while (run);
}
