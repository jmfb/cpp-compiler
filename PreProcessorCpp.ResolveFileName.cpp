#include "PreProcessorCpp.h"
#include <CRL/FileUtility.h>
#include <deque>
#include <stdexcept>

std::string PreProcessorCpp::ResolveFileName(
	bool systemInclude,
	const Position& position,
	const std::string& relativeFileName) const
{
	std::deque<std::string> directories = {
		R"(c:\save\program files\mingw\x86_64-w64-mingw32\include)",
		R"(c:\save\program files\mingw\lib\gcc\x86_64-w64-mingw32\4.7.2\include)",
		R"(c:\save\program files\mingw\lib\gcc\x86_64-w64-mingw32\4.7.2\include\c++)",
		R"(c:\save\program files\mingw\lib\gcc\x86_64-w64-mingw32\4.7.2\include\c++\x86_64-w64-mingw32)",
		R"(c:\save\code)"
	};

	auto currentDirectory = FSYS::GetFilePath(currentFileName);
	if (systemInclude)
		directories.push_back(currentDirectory);
	else
		directories.push_front(currentDirectory);

	for (auto& directory: directories)
	{
		auto fileName = FSYS::FormatPath(directory, relativeFileName);
		if (FSYS::FileExists(fileName))
			return fileName;
	}

	throw std::logic_error("Could not resolve file name: " + relativeFileName + "\n" + position.ToString());
}

