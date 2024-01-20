#pragma once

#include <cstdint>
#include <map>

class ComPanel
{
public:
    explicit ComPanel(std::map<uint8_t, uint8_t> settings);

    uint8_t forward(uint8_t input) const noexcept;
    uint8_t backward(uint8_t input) const noexcept;

private:
    std::map<uint8_t, uint8_t> settings;
};
