#include "enigma.h"

Enigma::Enigma(int seed, std::shared_ptr<ComPanel> comPanel) noexcept
    : comPanel(std::move(comPanel)), rotors{Rotor::buildRandom(seed), Rotor::buildRandom(seed + 1), Rotor::buildRandom(seed + 2)}, reflector(Reflector::buildRandom(seed + 3))
{
}

void Enigma::encrypt(std::istream &input, std::ostream &output)
{
    do
    {
        uint8_t symbol = input.get();
        if (input.eof())
            break;

        symbol = comPanel->forward(symbol);

        symbol = rotors[0].forward(symbol);
        symbol = rotors[1].forward(symbol);
        symbol = rotors[2].forward(symbol);

        symbol = reflector.reflect(symbol);

        symbol = rotors[2].backward(symbol);
        symbol = rotors[1].backward(symbol);
        symbol = rotors[0].backward(symbol);

        symbol = comPanel->backward(symbol);

        output.put(symbol);
        rotate();
    } while (true);
}

void Enigma::rotate()
{
    if (rotors[0].rotate())
        if (rotors[1].rotate())
            rotors[2].rotate();
}
