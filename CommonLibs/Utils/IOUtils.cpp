#include "IOUtils.h"
#include <fstream>
#include "ByteStream.h"
#include "..\Logger\Logger.h"

namespace utils {

	using namespace logger;

	bool DoesFileExist(const std::wstring& filePath)
	{
		std::ifstream file(filePath);
		auto result = file.good();
		file.close();
		return result;
	}

	std::streamoff GetFileSize(const std::wstring& filePath)
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

	void ReadFileToVector(const std::wstring& filePath, std::vector<uint8_t>& destBuffer)
	{
		LogAction(FULL_FUNC_INFO_W(L""));

		auto fileSize = GetFileSize(filePath);

		destBuffer.resize(fileSize);

		std::ifstream binaryFile;
		binaryFile.open(filePath, std::ios::binary);
		bool DEBUG_FILE_OPEN = binaryFile.is_open();

		binaryFile.read((char*)destBuffer.data(), destBuffer.size());
		binaryFile.close();
	}

	static bool IsDiskFile(const std::wstring& _str)
	{
		if (_str.size() < 2)
			return false;


		return (_str[1] == L':');
	};
}