#pragma once

#include <fstream>
#include <vector>

namespace file_helpers
{
    extern std::streamoff GetFileSize(const std::wstring& filePath);
    extern std::vector<uint8_t> ReadFileToVector(const std::wstring& filePath);
}