
#include "FileHelpers.h"

std::streamoff file_helpers::GetFileSize(const std::wstring& filePath)
{
    std::streampos begin, end;
    std::ifstream myfile(filePath, std::ios::binary);
    begin = myfile.tellg();
    myfile.seekg(0, std::ios::end);
    end = myfile.tellg();
    auto fileSize = end - begin;

    myfile.close();

    return fileSize;
}
