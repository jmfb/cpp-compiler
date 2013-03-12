#pragma once
#include <string>
#include <istream>
#include <cstddef>
#include <vector>
#include "Char.h"

class CharStream
{
public:
	CharStream() = delete;
	CharStream(std::istream& in, const std::string& name);
	CharStream(const CharStream& rhs) = delete;
	~CharStream() = default;

	CharStream& operator=(const CharStream& rhs) = delete;

	const std::string& GetName() const;
	bool IsEOF() const;
	void MoveFirst();
	void PutBack(Char ch);
	Char GetNext();
	Char ScanPast(const std::string& str);

	virtual Char InternalGetNext() = 0;

protected:
	std::istream& in;
	std::string name;
	std::size_t line = 1;
	std::size_t column = 1;
	std::vector<Char> buffer;
};

