#include "TransitionTable.h"
#include <fstream>
#include <sstream>
#include <stack>
#include <cstddef>
#include <stdexcept>
#include "SentenceToken.h"
#include "Sentence.h"

SentenceItemPtr TransitionTable::Parse(TokenStream& tokens, SymbolTable& symtab, std::ostream* debugDump)
{
	std::stack<std::size_t> state;
	state.push(0);
	std::stack<SentenceItemPtr> stack;
	for (auto token = tokens.GetNextToken(); ; )
	{
		if (debugDump != nullptr)
			(*debugDump) << "State " << state.top() << ", Token " << token.value << " : ";

		auto& entry = table[state.top()];
		auto action = entry.GetAction(token, symtab);
		switch(action.type)
		{
		case TransitionType::Shift:
			if (debugDump != nullptr)
				(*debugDump) << "Shift " << action.state << std::endl;
			stack.push(SentenceToken::Create(token));
			state.push(action.state);
			token = tokens.GetNextToken();
			break;

		case TransitionType::Reduce:
			{
				if (debugDump != nullptr)
					(*debugDump) << "Reduce " << action.nonTerminal << std::endl;
				auto item = Sentence::Create(action.nonTerminal);
				auto& sentence = *dynamic_cast<Sentence*>(item.get());
				for (auto index = 0ul; index < action.productionItemCount; ++index)
				{
					sentence.items.push_front(stack.top());
					stack.pop();
					state.pop();
				}
				stack.push(item);
				state.push(table[state.top()].GetGoto(action.nonTerminal));
			}
			break;

		case TransitionType::Accept:
			{
				auto item = stack.top();
				stack.pop();
				auto sentence = dynamic_cast<Sentence*>(item.get());
				if (sentence == nullptr)
					throw std::logic_error("Top of stack should be sentence.");
				if (!stack.empty())
					throw std::logic_error("Stack should be empty on acceptance.");
				sentence->Simplify();
				if (debugDump != nullptr)
					(*debugDump) << "Accept" << std::endl;
				return item;
			}
		}
	}
}

void TransitionTable::SaveToFile(const std::string& fileName) const
{
	std::ofstream out { fileName.c_str(), std::ios::binary };
	BinaryWriter writer { out };
	Write(writer);
}

void TransitionTable::LoadFromFile(const std::string& fileName)
{
	std::ifstream in { fileName.c_str(), std::ios::binary };
	BinaryReader reader { in };
	Read(reader);
}

void TransitionTable::Write(BinaryWriter& writer) const
{
	writer.Write(static_cast<long>(table.size()));
	for (auto& entry: table)
		entry.Write(writer);
}

void TransitionTable::Read(BinaryReader& reader)
{
	auto count = reader.ReadInt32();
	for (auto index = 0; index < count; ++index)
	{
		TransitionList entry;
		entry.Read(reader);
		table.push_back(entry);
	}
}

std::string TransitionTable::ToString() const
{
	std::ostringstream out;
	for (auto& entry: table)
		out << entry.ToString() << std::endl;
	return out.str();
}

