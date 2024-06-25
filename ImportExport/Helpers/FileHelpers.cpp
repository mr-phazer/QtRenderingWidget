
#include "..\..\Rldx\Rldx\Logging\Logging.h"
#include "ByteStream.h"
#include "FileHelpers.h"

bool file_helpers::DoesFileExist(const std::wstring& filePath)
{
	std::ifstream file(filePath);
	bool result = file.is_open();
	file.close();
	return result;
}

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

void file_helpers::ReadFileToVector(const std::wstring& filePath, std::vector<uint8_t>& destBuffer)
{
	logging::LogAction(FULL_FUNC_INFO(""));

	auto fileSize = GetFileSize(filePath);

	destBuffer.resize(fileSize);

	std::ifstream binaryFile;
	binaryFile.open(filePath, std::ios::binary);
	bool DEBUG_FILE_OPEN = binaryFile.is_open();

	binaryFile.read((char*)destBuffer.data(), destBuffer.size());
	binaryFile.close();
}
