#pragma once

#include <iostream>
#include <cmath>
#include <vector>

using BigInt = long long;

BigInt gcd(BigInt a, BigInt b);

bool prime(BigInt n);

BigInt fastPow(BigInt a, BigInt step, BigInt mod);

BigInt gcd(BigInt a, BigInt b)
{
    while (a != b)
    {
        if (a > b)
        {
            BigInt tmp = a;
            a = b;
            b = tmp;
        }
        b = b - a;
    }
    return a;
}

bool prime(BigInt n)
{
    for (BigInt i = 2; i <= sqrt(n); i++)
        if (n % i == 0)
            return false;
    return true;
}

BigInt fastPow(BigInt a, BigInt step, BigInt mod)
{
    if (step == 0)
    {
        return 1;
    }
    if (step % 2 == 1)
    {
        return (fastPow(a, step - 1, mod)) * a % mod;
    }
    else
    {
        BigInt b = fastPow(a, step / 2, mod);
        b %= mod;
        return (b * b) % mod;
    }
}
