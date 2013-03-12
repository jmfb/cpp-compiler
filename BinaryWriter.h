#pragma once
#include <iostream>
#include <string>
#include <cstddef>

class BinaryWriter
{
public:
	BinaryWriter() = delete;
	BinaryWriter(std::ostream& out);
	BinaryWriter(const BinaryWriter& rhs) = delete;
	~BinaryWriter() = default;

	BinaryWriter& operator=(const BinaryWriter& rhs) = delete;

	void Write(long value);
	void Write(std::size_t value);
	void Write(const std::string& value);

private:
	void WriteBinary(const void* data, std::size_t size);

private:
	std::ostream& out;
};

