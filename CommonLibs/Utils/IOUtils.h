#pragma once

#include <vector>
#include <string>

namespace utils {

	bool IsDiskFile(const std::wstring& _str);
	bool DoesFileExist(const std::wstring& filePath);
	//std::streamoff GetFileSize(const std::wstring& filePath);
	size_t GetFileSize(const std::wstring& strFileName);
	static inline size_t GetInputFileSize(std::ifstream& is);
	void ReadFileToVector(const std::wstring& filePath, std::vector<uint8_t>& destBuffer);

	std::wstring GetExePath();
	std::wstring GetDirFromFullPath(const std::wstring& _str);
	std::wstring GetFileExtension(const std::wstring& _str);
	std::wstring RemoveExtension(const std::wstring& _str);
	std::wstring GetFileFromFullPath(const std::wstring& _strInput);

	int fileOffsetToLineNumber(const std::wstring& _strFile, uint64_t _offset);
	int fileOffsetToLineNumberMem(const char* _pBuffer, size_t _size, size_t _file_offset);
}