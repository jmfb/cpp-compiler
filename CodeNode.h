#pragma once
#include <memory>

class CodeNode
{
public:
	CodeNode() = default;
	CodeNode(const CodeNode& rhs) = default;
	virtual ~CodeNode() = default;

	CodeNode& operator=(const CodeNode& rhs) = default;

	template <typename T>
	T& As()
	{
		return *dynamic_cast<T*>(this);
	}
};

typedef std::shared_ptr<CodeNode> CodeNodePtr;

