#pragma once
#include <memory>
#include <iostream>
#include "Position.h"
#include "CodeNode.h"

class Sentence;

class SentenceItem
{
public:
	SentenceItem() = default;
	SentenceItem(const SentenceItem& rhs) = delete;
	virtual ~SentenceItem() = default;

	SentenceItem& operator=(const SentenceItem& rhs);

	virtual const Position& GetPosition() const = 0;
	virtual void WriteXml(std::ostream& out) const = 0;

	template <typename T>
	T& As()
	{
		return *dynamic_cast<T*>(this);
	}

	template <typename T>
	T& AsCode();

	CodeNodePtr AsNode();

	const std::string& AsToken();
};

typedef std::shared_ptr<SentenceItem> SentenceItemPtr;

