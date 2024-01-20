#pragma once

#include <iostream>
#include <memory>
#include "rotor.h"
#include "reflector.h"
#include "companel.h"

class Enigma final
{
public:
    Enigma(int seed, std::shared_ptr<ComPanel> comPanel) noexcept;
    ~Enigma() = default;

    void encrypt(std::istream &input, std::ostream &output);

private:
    void rotate();

    std::shared_ptr<ComPanel> comPanel;
    std::array<Rotor, 3> rotors;
    Reflector reflector;
};
