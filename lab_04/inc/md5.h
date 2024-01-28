#pragma once

#include <string>
#include <vector>

class MD5
{
public:
    typedef unsigned int size_type; // must be 32bit

    MD5();
    MD5(const std::string &text);
    MD5(const char *input, size_t length);
    void update(const unsigned char *buf, size_type length);
    void update(const char *buf, size_type length);
    MD5 &finalize();
    std::string hexdigest() const;

private:
    void init();
    const static int blocksize = 64;

    void transform(const uint8_t block[blocksize]);
    static void decode(uint32_t output[], const uint8_t input[], size_type len);
    static void encode(uint8_t output[], const uint32_t input[], size_type len);

    bool finalized;
    uint8_t buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
    uint32_t count[2];         // 64bit counter for number of bits (lo, hi)
    uint32_t state[4];         // digest so far
    uint8_t digest[16];        // the result
};

std::string md5(const std::string str);
std::string md5(const std::vector<uint8_t> &input);
