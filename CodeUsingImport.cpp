#include "CodeUsingImport.h"
#include "CodeNodeRegister.h"

CodeUsingImport::CodeUsingImport(const CodeQualifiedName& name)
	: name(name)
{
}

namespace
{
	CodeNodeRegister usingImport
	{
		"using-import",
		{
			{
				"\"using\" <qualified-name> ';'",
				[](CodeNodeItems items)
				{
					return new CodeUsingImport
					{
						items[1]->AsCode<CodeQualifiedName>()
					};
				}
			}
		}
	};
}

