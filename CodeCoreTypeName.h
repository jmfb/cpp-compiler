#pragma once
#include "CodeNode.h"
#include "CodeCoreType.h"
#include "CodeBuiltInTypeName.h"
#include "CodeQualifiedName.h"

class CodeCoreTypeName : public CodeNode
{
public:
	CodeCoreTypeName(const CodeBuiltInTypeName& builtInTypeName);
	CodeCoreTypeName(const CodeQualifiedName& qualifiedName);
	CodeCoreTypeName(const CodeCoreTypeName& rhs) = default;

	CodeCoreTypeName& operator=(const CodeCoreTypeName& rhs) = default;
	
public:
	CodeCoreType type;
	CodeBuiltInTypeName builtInTypeName;
	CodeQualifiedName qualifiedName;
};

