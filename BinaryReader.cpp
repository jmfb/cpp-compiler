#include "BinaryReader.h"
#include <vector>
#include <winsock2.h>

BinaryReader::BinaryReader(std::istream& in)
	: in(in)
{
}

long BinaryReader::ReadInt32()
{
	auto value = 0l;
	ReadBinary(&value, sizeof(value));
	return ::ntohl(value);
}

std::size_t BinaryReader::ReadSize()
{
	static_assert(sizeof(std::size_t) == sizeof(long long), "Size must be 64 bit for byte order conversion.");
	auto upper = static_cast<std::size_t>(ReadInt32());
	auto lower = static_cast<std::size_t>(ReadInt32());
	return (upper << 32) | lower;
}

std::string BinaryReader::ReadString()
{
	std::vector<char> characters;
	characters.resize(ReadInt32());
	ReadBinary(characters.data(), characters.size());
	return { characters.data(), characters.size() };
}

void BinaryReader::ReadBinary(void* data, std::size_t size)
{
	in.read(reinterpret_cast<char*>(data), size);
}

