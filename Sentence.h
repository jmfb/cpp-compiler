#pragma once
#include <deque>
#include <string>
#include "SentenceItem.h"

class Sentence : public SentenceItem
{
public:
	Sentence() = delete;
	Sentence(const std::string& name);
	Sentence(const Sentence& rhs) = delete;
	~Sentence() noexcept = default;

	Sentence& operator=(const Sentence& rhs) = delete;

	const Position& GetPosition() const final;
	void WriteXml(std::ostream& out) const final;
	void Simplify();

	static SentenceItemPtr Create(const std::string& name);

public:
	std::string name;
	std::deque<SentenceItemPtr> items;
};

