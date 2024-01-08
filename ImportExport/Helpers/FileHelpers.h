#pragma once

#include <fstream>

namespace file_helpers
{
    static std::streamoff GetFileSize(const std::wstring& filePath);
}