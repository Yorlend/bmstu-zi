#pragma once

#include <cstdint>
#include <iostream>

class DesCryptor
{
public:
    explicit DesCryptor(uint64_t key) noexcept;

    void encrypt(std::istream& input, std::ostream& output) const;
    void decrypt(std::istream& input, std::ostream& output) const;

private:
    const uint64_t key;
};
