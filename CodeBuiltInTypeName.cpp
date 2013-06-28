#include "CodeBuiltInTypeName.h"
#include "CodeNodeRegister.h"

CodeBuiltInTypeName::CodeBuiltInTypeName(CodeBuiltInType type)
	: type(type)
{
}

#define MAP_TYPE(typeName,typeEnum) \
	{ \
		"\"" #typeName "\"", \
		[](CodeNodeItems) \
		{ \
			return new CodeBuiltInTypeName { CodeBuiltInType::typeEnum }; \
		} \
	}

namespace
{
	CodeNodeRegister builtInTypeName
	{
		"built-in-type-name",
		{
			MAP_TYPE(var, Var),
			MAP_TYPE(void, Void),
			MAP_TYPE(bool, Bool),
			MAP_TYPE(char, Char),
			MAP_TYPE(wchar, WideChar),
			MAP_TYPE(byte, Byte),
			MAP_TYPE(ushort, UnsignedShort),
			MAP_TYPE(short, Short),
			MAP_TYPE(uint, UnsignedInt),
			MAP_TYPE(int, Int),
			MAP_TYPE(ulong, UnsignedLong),
			MAP_TYPE(long, Long),
			MAP_TYPE(float, Float),
			MAP_TYPE(double, Double),
			MAP_TYPE(ldouble, LongDouble)
		}
	};
}

