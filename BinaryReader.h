#pragma once
#include <iostream>
#include <string>
#include <cstddef>

class BinaryReader
{
public:
	BinaryReader() = delete;
	BinaryReader(std::istream& in);
	BinaryReader(const BinaryReader& rhs) = delete;
	~BinaryReader() = default;

	BinaryReader& operator=(const BinaryReader& rhs) = delete;

	long ReadInt32();
	std::size_t ReadSize();
	std::string ReadString();

private:
	void ReadBinary(void* data, std::size_t size);

private:
	std::istream& in;
};

