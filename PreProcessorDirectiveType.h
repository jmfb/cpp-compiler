#pragma once

enum class PreProcessorDirectiveType
{
	Include,
	Define,
	Undefine,
	Line,
	Error,
	Warning,
	Pragma,
	Empty
};

