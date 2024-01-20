#include "aes.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "cfb.hpp"

int main(int argc, const char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "usage: " << argv[0] << " [-e|-d] <key> <input_file> <output_file>";
        return -1;
    }

    bool encrypt = strcmp(argv[1], "-e") == 0;

    uint8_t key_bytes[17];
    std::snprintf(reinterpret_cast<char *>(key_bytes), 17, "%016X", std::atoi(argv[2]));
    uint128_t key = uint128_t::pack_bytes(reinterpret_cast<uint8_t(&)[16]>(key_bytes));

    std::ifstream input(argv[3], std::ios::binary);
    std::ofstream output(argv[4], std::ios::binary);

    if (!input.is_open())
        throw std::runtime_error("input file not opened");
    if (!output.is_open())
        throw std::runtime_error("output file not opened");

    auto cryptor = std::make_shared<AESCryptor>(key);
    auto cfb = CipherFeedBackMode<AESCryptor>(cryptor, key);
    if (encrypt)
        cfb.encrypt(input, output);
    else
        cfb.decrypt(input, output);

    return 0;
}
