#pragma once

#include <fstream>
#include <vector>

namespace file_helpers
{
	bool DoesFileExist(const std::wstring& filePath);
	extern std::streamoff GetFileSize(const std::wstring& filePath);
	extern void ReadFileToVector(const std::wstring& filePath, std::vector<uint8_t>& dest);
}