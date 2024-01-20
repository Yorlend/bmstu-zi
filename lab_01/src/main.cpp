#include <iostream>
#include <fstream>
#include "enigma.h"

constexpr auto enigma_default_seed = 666;

int main(int argc, const char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1], std::ios::in | std::ios::binary);
    if (!input.is_open())
    {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    std::ofstream output(argv[2], std::ios::out | std::ios::binary);
    if (!output.is_open())
    {
        std::cerr << "Failed to open output file" << std::endl;
        return 1;
    }

    std::map<uint8_t, uint8_t> commutationMap = {
        {'A', 'B'},
        {'C', 'E'},
    };
    auto comPanel = std::make_shared<ComPanel>(commutationMap);

    Enigma enigma(enigma_default_seed, comPanel);

    enigma.encrypt(input, output);
    return 0;
}
