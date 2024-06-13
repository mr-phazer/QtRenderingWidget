#pragma once

#include <vector>
#include <string>

namespace utils {

	bool DoesFileExist(const std::wstring& filePath);
	std::streamoff GetFileSize(const std::wstring& filePath);
	void ReadFileToVector(const std::wstring& filePath, std::vector<uint8_t>& destBuffer);
}