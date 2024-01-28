#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "md5.h"
#include "RSA.hpp"

std::vector<uint8_t> readFile(const char *filename)
{
    std::ifstream file(filename, std::ios::binary);
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void writeSign(const char *filename, const std::vector<BigInt> &sign)
{
    std::stringstream buf;
    for (const auto num : sign)
        buf << std::hex << num;

    std::ofstream outf(filename, std::ios::trunc);
    for (int i = 0; i < 8; i++)
    {
        char tmp[32];
        buf.read(tmp, 32);
        outf.write(tmp, 32);
        outf << std::endl;
    }
}

std::string extractHash(const std::string &decrypted_signature)
{
    return decrypted_signature.substr(32);
}

int main(int argc, char *argv[])
{
    // Input
    if (argc != 2)
    {
        std::cout << "Мало аргументов!" << std::endl;
        return 0;
    }
    const char *filename = argv[1];
    std::cout << "Название файла <" << filename << ">" << std::endl;

    auto input = readFile(filename);

    // Hash
    const auto hash = md5(input);

    std::cout << "MD5 для \"" << filename << "\" <" << hash << ">" << std::endl
              << std::endl;

    // Signature creation
    std::vector<long long> vec(hash.size());
    for (const auto c : hash)
        vec.push_back(static_cast<long long>(c));

    // Keys
    Keys keys = calculateRSAKeys();
    const auto sign = cryptMessage(vec, keys._private);
    writeSign("sign.txt", sign);

    std::cout << "ЭЦП <";
    for (auto i = 0; i < sign.size(); ++i)
    {
        std::cout << sign[i];
        if (i != sign.size() - 1)
            std::cout << " ";
    }
    std::cout << ">" << std::endl
              << std::endl;

    const auto input2 = readFile(filename);

    // Signature verification
    const auto originalHash = md5(input2);

    auto signatureHash = encryptMessage(sign, keys._public);
    signatureHash = extractHash(signatureHash);

    std::cout << "Хеш ЭЦП <" << signatureHash << ">" << std::endl;
    std::cout << "Хеш документа <" << originalHash << ">" << std::endl
              << std::endl;

    if (originalHash == signatureHash)
        std::cout << "ОК" << std::endl;
    else
        std::cout << "Ошибка!" << std::endl;

    return 0;
}
