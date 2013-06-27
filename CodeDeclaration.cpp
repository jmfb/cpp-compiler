#include "CodeDeclaration.h"
#include "Sentence.h"

CodeDeclaration::CodeDeclaration(CodeTypeName type, const std::string& name)
	: type(type), name(name)
{
}

CodeNodeFactory::NonTerminalEntry CodeDeclaration::creator
{
	"declaration",
	{
		{
			"<type-name> id ';'",
			[](CodeNodeFactory::Items items)
			{
				return CodeNodePtr
				{
					new CodeDeclaration
					{
						items[0]->AsCode<CodeTypeName>(),
						items[1]->AsToken()
					}
				};
			}
		}
	}
};

