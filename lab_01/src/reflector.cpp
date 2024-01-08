#include "reflector.h"
#include <random>
#include <algorithm>

uint8_t Reflector::reflect(uint8_t input) const noexcept
{
    return wires[input];
}

Reflector Reflector::buildRandom(int seed) noexcept
{
    std::mt19937 random_generator(seed);
    std::array<uint8_t, 256> wires;
    wires.fill(0);

    int zero_pos = random_generator() % 256;
    wires[0] = zero_pos;

    int i = 0;
    while (1)
    {
        while (i < 256 && wires[i] != 0)
            i++;

        if (i >= 256)
            break;
        
        if (i == zero_pos)
        {
            i++;
            continue;
        }

        int pos = random_generator() % 256;
        while (wires[pos] != 0 || pos == zero_pos)
            pos = (pos + 1) % 256;
        wires[pos] = i;
        wires[i] = pos;
    }
    
    return Reflector(wires);
}

Reflector::Reflector(std::array<uint8_t, 256> wires) noexcept
    : wires(std::move(wires))
{}
