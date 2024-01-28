#pragma once

#include <limits>
#include <vector>
#include <algorithm>
#include <iostream>
#include <bit>
#include <bitset>

class LzwDecompressService
{
	uint8_t getBitsToRepresentInteger(uint32_t x)
	{
		return std::numeric_limits<uint32_t>::digits - std::countl_zero(x);
	}

	uint32_t readCode(
		const std::vector<uint8_t> &inputFile,
		int &bytesRead,
		uint8_t &bitsRead,
		uint8_t bitsToRead)
	{
		uint32_t code = 0;
		uint8_t bitsReadTotal = 0;

		while (bitsToRead > 0)
		{
			uint8_t currentByte = inputFile[bytesRead];
			uint8_t bitsCanReadFromCurrentByte = 8 - bitsRead;
			uint8_t bitsNeedToRead = std::min(bitsToRead, bitsCanReadFromCurrentByte);
			uint8_t bitsDontNeedToRead = 8 - bitsNeedToRead - bitsRead;
			uint8_t strippedDontNeedBits = currentByte << bitsDontNeedToRead;
			uint8_t newCodePart = (strippedDontNeedBits) >> (bitsDontNeedToRead + bitsRead);

			code |= ((uint32_t)newCodePart) << bitsReadTotal;

			bitsReadTotal += bitsNeedToRead;
			bitsRead = (8 - bitsDontNeedToRead) % 8;

			bitsToRead -= std::min(bitsCanReadFromCurrentByte, bitsToRead);

			if (bitsDontNeedToRead == 0)
			{
				bytesRead++;
			}
		}
		return code;
	}

	std::string vecToStr(const std::vector<uint8_t> &vec)
	{
		std::string str;
		for (unsigned char i : vec)
		{
			str.push_back(i);
		}
		return str;
	}

	void printDict(const std::vector<std::vector<uint8_t>> &dict)
	{
		std::cout << "\u001b[35m" << std::endl;
		for (size_t i = 255; i < dict.size(); i++)
		{
			std::cout << "dict: " << i << " : " << vecToStr(dict[i]) << std::endl;
		}
		std::cout << "\u001b[0m" << std::endl;
	}

public:
	void decompress(const std::vector<uint8_t> &inputFile, std::vector<uint8_t> &outputFile)
	{
		std::vector<std::vector<uint8_t>> dict(256, std::vector<uint8_t>());
		for (int i = 0x00; i <= 0xFF; i++)
		{
			dict[i].push_back(i);
		}

		int coder = 0xFF + 1;
		std::vector<uint8_t> current;

		int bytesRead = 0;
		uint8_t bitsRead = 0;
		int bitsToRead = getBitsToRepresentInteger(coder);
		while (static_cast<size_t>(bytesRead + (bitsToRead / 8 + std::min(bitsToRead % 8, 1)) - 1) < inputFile.size())
		{
			bitsToRead = getBitsToRepresentInteger(coder);
			uint32_t code = readCode(inputFile, bytesRead, bitsRead, bitsToRead);

			for (const auto &x : dict[code])
			{
				current.push_back(x);
			}

			for (const auto &x : current)
			{
				outputFile.push_back(x);
			}
			dict.push_back(current);

			bitsToRead = getBitsToRepresentInteger(coder);
			if (static_cast<size_t>(bytesRead + (bitsToRead / 8 + std::min(bitsToRead % 8, 1)) - 1) < inputFile.size())
			{

				int tmpBytesRead = bytesRead;
				uint8_t tmpBitsRead = bitsRead;

				uint32_t code2 = readCode(inputFile, tmpBytesRead, tmpBitsRead, bitsToRead);
				dict[coder].push_back(dict[code2][0]);
			}

			coder++;
			current.clear();
		}
	}
};
