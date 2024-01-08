#pragma once

#include <cstdint>
#include <array>

class Reflector final
{
public:
    Reflector(const Reflector &) = delete;
    Reflector(Reflector &&) = default;
    Reflector &operator=(const Reflector &) = delete;
    Reflector &operator=(Reflector &&) = default;
    ~Reflector() = default;

    uint8_t reflect(uint8_t input) const noexcept;

    static Reflector buildRandom(int seed) noexcept;

private:
    explicit Reflector(std::array<uint8_t, 256> wires) noexcept;
    
    std::array<uint8_t, 256> wires;
};
