#include "companel.h"
#include <stdexcept>

ComPanel::ComPanel(std::map<uint8_t, uint8_t> settings)
{
    // check that settings are valid bidirectional mapping
    for (const auto [key, val] : settings)
    {
        if (settings.count(val) == 0)
            settings[val] = key;
        else if (settings.at(val) != key)
            throw std::runtime_error("invalid commutation settings");
    }

    this->settings = std::move(settings);
}

uint8_t ComPanel::forward(uint8_t input) const noexcept
{
    if (settings.count(input) == 0)
        return input;

    return settings.at(input);
}

uint8_t ComPanel::backward(uint8_t input) const noexcept
{
    if (settings.count(input) == 0)
        return input;

    return settings.at(input);
}
