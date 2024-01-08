#include "rotor.h"
#include <algorithm>
#include <random>

uint8_t Rotor::forward(uint8_t input) const noexcept
{
    return forward_pass[input % capacity];
}

uint8_t Rotor::backward(uint8_t input) const noexcept
{
    return backward_pass[input % capacity];
}

bool Rotor::rotate() noexcept
{
    auto tmp = forward_pass[0];
    std::copy(forward_pass.begin() + 1, forward_pass.end(), forward_pass.begin());
    forward_pass[capacity - 1] = tmp;
    for (int pos = 0; pos < capacity; pos++)
        backward_pass[forward_pass[pos]] = pos;

    rotation = (rotation + 1) % capacity;
    return rotation == 0;
}

Rotor Rotor::buildRandom(int seed)
{
    std::mt19937 random_generator(seed);
    std::array<uint8_t, 256> forward_pass;
    std::array<uint8_t, 256> backward_pass;

    for (int pos = 0; pos < capacity; pos++)
        forward_pass[pos] = pos;

    std::shuffle(forward_pass.begin(), forward_pass.end(), random_generator);

    for (int pos = 0; pos < capacity; pos++)
        backward_pass[forward_pass[pos]] = pos;

    return Rotor(forward_pass, backward_pass);
}

Rotor::Rotor(std::array<uint8_t, 256> forward_pass, std::array<uint8_t, 256> backward_pass)
    : forward_pass(forward_pass)
    , backward_pass(backward_pass)
{}
