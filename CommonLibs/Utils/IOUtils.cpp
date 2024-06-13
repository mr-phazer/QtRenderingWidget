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

	std::wstring GetExePath()
	{
		WCHAR path[MAX_PATH];
		GetModuleFileNameW(NULL, path, MAX_PATH);
		std::wstring exePath = GetDirFromFullPath(path);

		return exePath;
	}

	std::wstring GetDirFromFullPath(const std::wstring& _str)
	{
		std::wstring strTemp = _str;

		if (strTemp == L"")
			return L"";

		size_t i = 0;
		int i1 = 0;
		size_t index = strTemp.size();
		size_t cut = 0;;

		while (strTemp[index] != L'\\' && strTemp[index] != L'/' && index > 0)
		{
			index--;
			cut++;
		}
		index--;

		if (index == 0)
			return strTemp;

		strTemp.erase(index + 2, cut - 1);

		return strTemp;
	}

	std::wstring GetFileExtension(const std::wstring& _str)
	{
		std::wstring out = _str;

		auto dotPosition = out.find('.');
		if (dotPosition == std::wstring::npos) // no dot, return unchanged
			return out;

		out.erase(0, dotPosition + 1);

		return out;


		if (_str == L"")
			return L"";

		bool bFounDot = false;
		size_t len = _str.length();
		size_t index = len - 1;

		std::wstring strTemp = L"";

		// move the index back to where the extension starts
		while (_str[index] != L'.' && index > 0)
		{
			index--;
		}

		if (index <= 0)
			return L"";

		strTemp = &_str[index + 1];

		return strTemp;
	}

	std::wstring RemoveExtension(const std::wstring& _str)
	{
		if (_str == L"")
			return L"";

		bool bFounDot = false;
		size_t len = _str.length();
		size_t index = len - 1;

		std::wstring strTemp = _str;

		// move the index back to where the extension starts
		while (strTemp[index] != L'.' && index > 0)
		{
			index--;
		}

		if (index <= 0)
			return strTemp;

		strTemp.erase(index, len - index);

		return strTemp;
	}

	std::wstring GetFileFromFullPath(const std::wstring& _strInput)
	{
		if (_strInput.empty())
			return L"";

		const wchar_t* _csz = _strInput.c_str();
		size_t len = _strInput.size();
		size_t index = len - 1;

		// move the index back to where the extension starts
		while (_csz[index] != L'\\' && _csz[index] != L'/' && index > 0)
		{
			index--;
		}

		if (index < 0)
			return L"";

		if (index == 0) // if there is no path in front of file name½
			return &_csz[index];

		return &_csz[index + 1];
	}

}