#pragma once

#include "cipherchainer.hpp"

template <Cryptor E>
class CipherFeedBackMode final : public BaseChipherMode<E>
{
public:
    CipherFeedBackMode(std::shared_ptr<E> cryptor, uint128_t initVector)
        : BaseChipherMode<E>(cryptor), initVector(initVector) {}

    virtual void encrypt(std::istream &input, std::ostream &output) override;
    virtual void decrypt(std::istream &input, std::ostream &output) override;

private:
    uint128_t initVector;
};

template <Cryptor E>
void CipherFeedBackMode<E>::encrypt(std::istream &input, std::ostream &output)
{
    bool run = true;
    uint128_t nextInputBlock = initVector;
    while (run)
    {
        auto [block, extended] = BaseChipherMode<E>::readExtend(input);
        run = !extended;
        block ^= BaseChipherMode<E>::getCryptor().encrypt(nextInputBlock);
        nextInputBlock = block;
        BaseChipherMode<E>::write(output, block);
    };
}

template <Cryptor E>
void CipherFeedBackMode<E>::decrypt(std::istream &input, std::ostream &output)
{
    auto nextInputBlock = initVector;
    std::optional<uint128_t> writeQueue;
    while (true)
    {
        auto block = BaseChipherMode<E>::read(input);
        if (!block)
            break;

        auto res = *block ^ BaseChipherMode<E>::getCryptor().encrypt(nextInputBlock);
        nextInputBlock = *block;

        if (writeQueue)
            BaseChipherMode<E>::write(output, *writeQueue);
        writeQueue = std::make_optional(res);
    }

    if (writeQueue)
        BaseChipherMode<E>::writeRetract(output, *writeQueue);
}
