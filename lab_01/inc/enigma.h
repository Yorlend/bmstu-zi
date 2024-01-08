#pragma once

#include <iostream>
#include "rotor.h"
#include "reflector.h"

class Enigma final
{
public:
    explicit Enigma(int seed) noexcept;
    ~Enigma() = default;

    void encrypt(std::istream& input, std::ostream& output);

private:
    void rotate();

    std::array<Rotor, 3> rotors;
    Reflector reflector;
};
