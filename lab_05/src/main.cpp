#include <iostream>
#include <fstream>
#include <iomanip>
#include "compress.h"
#include "decompress.h"

void printUsage()
{
    printf("lzw {compress | decompress} {source} {destination}");
}

int main(int argc, char **argv)
{

    if (argc != 4)
    {
        printUsage();
        return 1;
    }

    // Decide encode or decode
    char *op = argv[1];

    const char *inputPath = argv[2];
    const char *outputPath = argv[3];

    std::ifstream inFile(inputPath, std::ios::binary);
    std::ofstream outFile(outputPath, std::ios::binary);

    if (!inFile || !outFile)
    {
        std::cout << "file stream error" << std::endl;
        return 1;
    }

    inFile.seekg(0, std::ios::end);
    long long fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<uint8_t> inData(fileSize);
    std::vector<uint8_t> outData;
    inFile.read((char *)(&inData[0]), fileSize);

    if (std::string(op) == "compress")
    {
        LzwCompressService compressService;
        compressService.compress(inData, outData);
        auto size = float(inData.size()) / outData.size();
        std::cout << "Коэффициент compress: " << std::setprecision(2) << std::fixed << size << std::endl;
    }
    else if (std::string(op) == "decompress")
    {
        LzwDecompressService decompressService;
        decompressService.decompress(inData, outData);
        auto size = float(outData.size()) / inData.size();
        std::cout << "Коэффициент decompress: " << std::setprecision(2) << std::fixed << size << std::endl;
    }
    else
    {
        printUsage();
        return 1;
    }
    outFile.write((char *)(&outData[0]), outData.size());

    return 0;
}
