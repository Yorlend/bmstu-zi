#pragma once

#include <memory>
#include <optional>
#include <iostream>
#include "uint128_t.h"

template <typename T>
concept Cryptor = requires(T e, uint128_t key, uint128_t block) {
    {
        e.encrypt(block)
    } -> std::convertible_to<uint128_t>;
    {
        e.decrypt(block)
    } -> std::convertible_to<uint128_t>;
};

template <Cryptor E>
class BaseChipherMode
{
public:
    BaseChipherMode(std::shared_ptr<E> cryptor) : cryptor(std::move(cryptor)) {}
    virtual ~BaseChipherMode() = default;

    virtual void encrypt(std::istream &input, std::ostream &output) = 0;
    virtual void decrypt(std::istream &input, std::ostream &output) = 0;

protected:
    E &getCryptor() const { return *cryptor; }

    // returns true if block was extended and there is no more input
    std::pair<uint128_t, bool> readExtend(std::istream &input) const;
    std::optional<uint128_t> read(std::istream &input) const;
    void write(std::ostream &output, uint128_t block) const;
    void writeRetract(std::ostream &output, uint128_t block) const;

private:
    void extendBuffer(uint8_t *buffer, size_t nread) const;

    std::shared_ptr<E> cryptor;
};

template <Cryptor E>
std::pair<uint128_t, bool> BaseChipherMode<E>::readExtend(std::istream &input) const
{
    uint8_t buffer[16];
    input.read(reinterpret_cast<char *>(buffer), 16);
    auto nread = input.gcount();
    bool extended = nread < 16;
    if (extended)
        extendBuffer(buffer, nread);
    return std::make_pair(uint128_t::pack_bytes(buffer), extended);
}

template <Cryptor E>
std::optional<uint128_t> BaseChipherMode<E>::read(std::istream &input) const
{
    uint8_t buffer[16];
    input.read(reinterpret_cast<char *>(buffer), 16);
    auto nread = input.gcount();
    if (nread == 0)
        return std::nullopt;
    else if (nread == 16)
        return std::make_optional(uint128_t::pack_bytes(buffer));
    else
        throw std::runtime_error("invalid input stream");
}

template <Cryptor E>
void BaseChipherMode<E>::write(std::ostream &output, uint128_t block) const
{
    output.write(reinterpret_cast<const char *>(block.bytes), 16);
}

template <Cryptor E>
void BaseChipherMode<E>::writeRetract(std::ostream &output, uint128_t block) const
{
    output.write(reinterpret_cast<const char *>(block.bytes), block.bytes[15]);
}

template <Cryptor E>
void BaseChipherMode<E>::extendBuffer(uint8_t *buffer, size_t nread) const
{
    buffer[15] = static_cast<uint8_t>(nread);
}
