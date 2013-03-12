#include "BinaryWriter.h"
#include <winsock2.h>

BinaryWriter::BinaryWriter(std::ostream& out)
	: out(out)
{
}

void BinaryWriter::Write(long value)
{
	auto orderedBytes = ::htonl(value);
	WriteBinary(&orderedBytes, sizeof(orderedBytes));
}

void BinaryWriter::Write(std::size_t value)
{
	static_assert(sizeof(std::size_t) == sizeof(long long), "Size must be 64 bit for byte order conversion.");
	auto upper = static_cast<long>((value & 0xffffffff00000000) >> 32);
	auto lower = static_cast<long>(value & 0x00000000ffffffff);
	Write(upper);
	Write(lower);
}

void BinaryWriter::Write(const std::string& value)
{
	auto size = static_cast<long>(value.size());
	Write(size);
	WriteBinary(value.data(), size);
}

void BinaryWriter::WriteBinary(const void* data, std::size_t size)
{
	out.write(reinterpret_cast<const char*>(data), size);
}

