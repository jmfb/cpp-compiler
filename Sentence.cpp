#include "Sentence.h"

Sentence::Sentence(const std::string& name)
	: name(name)
{
}

const Position& Sentence::GetPosition() const
{
	static const Position nullPosition { "", 0, 0 };
	if (items.empty())
		return nullPosition;
	return items.front()->GetPosition();
}

void Sentence::WriteXml(std::ostream& out) const
{
	out << "<" << name << ">";
	for (auto& item: items)
		item->WriteXml(out);
	out << "</" << name << ">";
}

bool EndsWith(const std::string& value, const std::string& suffix)
{
	return value.find(suffix) == (value.size() - suffix.size());
}

void Sentence::Simplify()
{
	for (auto index = 0ul; index < items.size(); )
	{
		auto item = items[index];
		auto sentence = dynamic_cast<Sentence*>(item.get());
		if (sentence == nullptr)
		{
			++index;
			continue;
		}

		if (EndsWith(sentence->name, "'") ||
			EndsWith(sentence->name, "*") ||
			EndsWith(sentence->name, "-opt") ||
			(EndsWith(name, "-list") && sentence->name == name))
		{
			items.erase(items.begin() + index);
			items.insert(items.begin() + index, sentence->items.begin(), sentence->items.end());
		}
		else
		{
			sentence->Simplify();
			++index;
		}
	}
}

SentenceItemPtr Sentence::Create(const std::string& name)
{
	return SentenceItemPtr { new Sentence { name } };
}

