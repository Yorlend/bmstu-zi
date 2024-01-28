#pragma once

#include "utils.hpp"

#include <algorithm>
#include <exception>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define PRIMES_FILENAME "primes.txt"

using BigIntPair = std::pair<BigInt, BigInt>;

struct Keys
{
    BigIntPair _public;
    BigIntPair _private;
};

Keys calculateRSAKeys();

BigInt cryptData(BigInt data, BigIntPair _publicKey);

BigInt encryptData(BigInt data, BigIntPair _privateKey);

std::vector<BigInt> cryptMessage(std::vector<BigInt> data, BigIntPair _publicKey);

std::string encryptMessage(std::vector<BigInt> data, BigIntPair _privateKey);

Keys calculateRSAKeys()
{
    std::vector<BigInt> primes(1034);
    std::ifstream fin(PRIMES_FILENAME);
    for (int i = 0; i < 1033; i++)
    {
        int temp;
        fin >> temp;
        primes[i] = temp;
    }

    BigInt p = primes[rand() % 1033];
    BigInt q = primes[rand() % 1033];

    BigInt n = p * q;

    BigInt functionE = (p - 1) * (q - 1);

    BigInt e = 1;
    for (BigInt i = functionE - 1; i > 0; --i)
    {
        if (gcd(i, functionE) == 1 && prime(i))
        {
            e = i;
            break;
        }
    }

    BigInt d;
    for (BigInt i = 0;; ++i)
    {
        if ((BigInt)i * (BigInt)e % (BigInt)functionE == 1)
        {
            d = i;
            break;
        }
    }

    Keys keys{BigIntPair{e, n}, BigIntPair{d, n}};
    return keys;
}

BigInt cryptData(BigInt data, BigIntPair _publicKey)
{
    return fastPow(data, _publicKey.first, _publicKey.second);
}

BigInt encryptData(BigInt data, BigIntPair _privateKey)
{
    return fastPow(data, _privateKey.first, _privateKey.second);
}

std::vector<BigInt> cryptMessage(std::vector<BigInt> data, BigIntPair _publicKey)
{
    std::vector<BigInt> cryptedMessage;
    cryptedMessage.reserve(data.size());
    for (auto element : data)
        cryptedMessage.push_back(cryptData(element, _publicKey));

    return cryptedMessage;
}

std::string encryptMessage(std::vector<BigInt> data, BigIntPair _privateKey)
{
    std::string encryptedMessage;
    for (BigInt element : data)
        encryptedMessage.push_back(encryptData(element, _privateKey));

    return encryptedMessage;
}
