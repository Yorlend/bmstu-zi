#pragma once

#include <bitset>
#include <limits>
#include <vector>
#include <unordered_map>
#include <istream>
#include <bit>
#include <iostream>

struct TrieNode
{
	uint32_t code;
	std::unordered_map<uint8_t, TrieNode> children;
};

class LzwCompressService
{
	TrieNode *rootNode;
	std::vector<std::pair<uint32_t, uint8_t>> *tempOut;

	void initializeDictionary()
	{
		rootNode->code = 0;
		for (int i = 0x00; i <= 0xFF; i++)
		{
			rootNode->children[i].code = i;
			rootNode->children[i].children.clear();
		}
	}

	uint8_t getBitsToRepresentInteger(uint32_t x)
	{
		return std::numeric_limits<uint32_t>::digits - std::countl_zero(x);
	}

	void writeBits(std::vector<uint8_t> &out)
	{
		uint8_t usedBits = 0;
		uint8_t currentByte = 0;

		for (const auto &x : *tempOut)
		{
			int8_t bitsToWrite = x.second;
			uint32_t code = x.first;
			while (bitsToWrite > 0)
			{
				uint8_t canWriteToByte = 8 - usedBits;
				currentByte |= code << usedBits;
				usedBits = bitsToWrite > canWriteToByte ? 0 : bitsToWrite + usedBits;
				code >>= canWriteToByte;
				if (usedBits == 0)
				{
					out.push_back(currentByte);
					currentByte = 0;
				}
				bitsToWrite -= canWriteToByte;
			}
		}
		if (usedBits != 0)
		{
			out.push_back(currentByte);
		}
	}

	int getDictSize(TrieNode *node = nullptr)
	{
		if (node == nullptr)
		{
			node = rootNode;
		}

		int sum = 1;
		for (auto &x : node->children)
		{
			sum += getDictSize(&x.second);
		}

		return sum;
	}

	// writes to internal tempOut
	void compressInternal(const std::vector<uint8_t> &inputFile)
	{
		initializeDictionary();

		std::vector<uint8_t> currentSubsequence;
		size_t nextIndex = 0;
		uint8_t nextByte;
		TrieNode *currentNode = rootNode;

		int code = 0xFF + 1;

		while (nextIndex < inputFile.size())
		{
			nextByte = inputFile[nextIndex];
			if (currentNode->children.contains(nextByte))
			{
				currentNode = &currentNode->children[nextByte];
				nextIndex++;
			}
			else
			{
				tempOut->emplace_back(currentNode->code, getBitsToRepresentInteger(code));
				currentNode->children[nextByte].code = code;
				code++;
				currentNode = rootNode;
			}
		}

		if (currentNode != rootNode)
		{
			tempOut->emplace_back(currentNode->code, getBitsToRepresentInteger(code));
		}

		std::cout << "dict size: " << getDictSize() << std::endl;
	}

public:
	void compress(const std::vector<uint8_t> &inputFile, std::vector<uint8_t> &outputFile)
	{
		compressInternal(inputFile);
		writeBits(outputFile);
	}

	LzwCompressService()
	{
		rootNode = new TrieNode();
		tempOut = new std::vector<std::pair<uint32_t, uint8_t>>();
	}

	~LzwCompressService()
	{
		delete rootNode;
		delete tempOut;
	}
};
