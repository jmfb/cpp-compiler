#pragma once
#include <memory>
#include <iostream>
#include "Position.h"

class SentenceItem
{
public:
	SentenceItem() = default;
	SentenceItem(const SentenceItem& rhs) = delete;
	virtual ~SentenceItem() = default;

	SentenceItem& operator=(const SentenceItem& rhs);

	virtual const Position& GetPosition() const = 0;
	virtual void WriteXml(std::ostream& out) const = 0;
};

typedef std::shared_ptr<SentenceItem> SentenceItemPtr;

