#pragma once

#include <vector>
#include <string>

namespace utils {

	bool DoesFileExist(const std::wstring& filePath);
	std::streamoff GetFileSize(const std::wstring& filePath);
	void ReadFileToVector(const std::wstring& filePath, std::vector<uint8_t>& destBuffer);
	static bool IsDiskFile(const std::wstring& _str)
	{
		if (_str.size() < 2)
			return false;


		return (_str[1] == L':');
	};

	std::wstring GetExePath();
	std::wstring GetDirFromFullPath(const std::wstring& _str);
	std::wstring GetFileExtension(const std::wstring& _str);
	std::wstring RemoveExtension(const std::wstring& _str);
	std::wstring GetFileFromFullPath(const std::wstring& _strInput);

}