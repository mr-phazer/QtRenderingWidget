
#include "FileHelpers.h"

std::streamoff file_helpers::GetFileSize(const std::wstring& filePath)
{
    std::streampos beginPos, endPos;
    std::ifstream binaryFile(filePath, std::ios::binary);
    
    beginPos = binaryFile.tellg();
    binaryFile.seekg(0, std::ios::end);
    endPos = binaryFile.tellg();
    
    auto fileSize = endPos - beginPos;

    binaryFile.close();

    return fileSize;
}

std::vector<uint8_t> file_helpers::ReadFileToVector(const std::wstring& filePath)
{   
    std::vector<uint8_t> destBuffer(GetFileSize(filePath));
    
    std::ifstream binaryFile;
    binaryFile.open(filePath, std::ios::binary);
    bool DEBUG_FILE_OPEN = binaryFile.is_open();
    binaryFile.read((char*)destBuffer.data(), destBuffer.size());
    binaryFile.close();

    return destBuffer;
}
