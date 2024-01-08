#pragma once

#include <cstdint>
#include <array>

class Rotor final
{
public:
    Rotor(const Rotor &) = delete;
    Rotor(Rotor &&) = default;
    Rotor &operator=(const Rotor &) = delete;
    Rotor &operator=(Rotor &&) = default;
    ~Rotor() = default;

    uint8_t forward(uint8_t input) const noexcept;
    uint8_t backward(uint8_t input) const noexcept;

    /** returns true on full rotation */
    bool rotate() noexcept;

    static Rotor buildRandom(int seed);

    static const int capacity = 256;

private:
    Rotor(std::array<uint8_t, 256> forward_pass, std::array<uint8_t, 256> backward_pass);

    int rotation = 0;

    std::array<uint8_t, 256> forward_pass;
    std::array<uint8_t, 256> backward_pass;
};
