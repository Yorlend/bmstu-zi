#pragma once

#include "uint128_t.h"

class AESCryptor final
{
public:
    AESCryptor(uint128_t key) : key(key) {}

    uint128_t encrypt(uint128_t data);
    uint128_t decrypt(uint128_t data);

private:
    uint128_t key;
};
