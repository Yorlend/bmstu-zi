#include "des.h"
#include <fstream>
#include <cstring>

static void print_usage(const char* arg0)
{
    std::cerr << "Usage: " << arg0 << " <key> -e|-d <input file> <output file>" << std::endl;
}

int main(int argc, const char* argv[])
{
    if (argc != 5)
    {
        print_usage(argv[0]);
        return 1;
    }
    if (strcmp(argv[2], "-e") != 0 && strcmp(argv[2], "-d") != 0)
    {
        print_usage(argv[0]);
        return 1;
    }
    uint64_t key;
    if (sscanf(argv[1], "%lx", &key) != 1)
    {
        std::cerr << "failed to parse key" << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    std::ifstream input_file(argv[3], std::ios::binary);
    if (!input_file.is_open())
    {
        std::cerr << "failed to open input file" << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    std::ofstream output_file(argv[4], std::ios::binary);
    if (!output_file.is_open())
    {
        std::cerr << "failed to open output file" << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    DesCryptor desCryptor(key);

    if (strcmp(argv[2], "-e") == 0)
        desCryptor.encrypt(input_file, output_file);
    else
        desCryptor.decrypt(input_file, output_file);
    
    return 0;
}
