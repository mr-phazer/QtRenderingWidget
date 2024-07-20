#include <fstream>

#include <CommonLibs\Logger\Logger.h>
#include "IOUtils.h"

namespace utils {

	using namespace std;
	using namespace logging;

	bool IsDiskFile(const std::wstring& _str)
	{
		if (_str.size() < 2)
			return false;

		return (_str[1] == L':');
	};

	bool DoesFileExist(const std::wstring& filePath)
	{
		std::ifstream file(filePath);
		bool result = file.is_open();
		file.close();
		return result;
	}

	/*
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
	}*/

	size_t GetFileSize(const std::wstring& strFileName)
	{
		struct _stat stat_buf {};
		int rc = _wstat(strFileName.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : 0;
	}

	static inline size_t GetInputFileSize(std::ifstream& is)
	{
		is.seekg(0, std::ios_base::end);
		std::size_t size = is.tellg();
		is.seekg(0, std::ios_base::beg);
		return size;
	}

	/*

	size_t GetFileSize(const std::string& strFileName)
	{
		struct _stat stat_buf {};
		int rc = _stat(strFileName.c_str(), &stat_buf);
		return rc == 0 ? stat_buf.st_size : 0;

		std::ifstream in(strFileName, std::ifstream::ate | std::ifstream::binary);
		return in.tellg();

		streampos begin, end;
		ifstream ifsFile(strFileName, ios::in | ios::binary);
		/* if ( ifsFile.fail() ) {
			 this->m_error_code = predefined_error_codes::FILE_NOT_FOUND;
			 if (window::getMainWindow())
				 window::getOKErrorBox(window::getMainWindow(), "Error!", "File not opened.");
			 else
				 window::getOKErrorBox(NULL, "Error!", "File not opened.");

			 writeDebugMessage("rmv2_file::_getFileSize: File not opened");

			 return false;
		}///

		begin = ifsFile.tellg();
		ifsFile.seekg(0, ios::end);
		end = ifsFile.tellg();
		ifsFile.close();

		size_t rEnd = end;
		size_t rBegin = begin;

		//if (rEnd - rBegin >= 0) {
		//	this->m_error_code = predefined_error_codes::FILE_SIZE_ZERO;
		//	if (window::getMainWindow())
		//		window::getOKErrorBox(window::getMainWindow(), "Error!", "File size = 0!");
		//	else
		//		window::getOKErrorBox(NULL, "Error!", "File size = 0!");

		//	writeDebugMessage("rmv2_file::_getFileSize: File size = 0!");

		//	return false;
		//}

		return (rEnd - rBegin);
	}
	*/

	void ReadFileToVector(const std::wstring& filePath, std::vector<uint8_t>& destBuffer)
	{
		LogAction(L"");

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


	int fileOffsetToLineNumber(const std::wstring& _strFile, uint64_t _offset)
	{
		std::ifstream in(_strFile, ios::binary);
		if (!in.is_open())
			return 0;

		char ch = 0;
		uint64_t offset = 0;
		int line_number = 1;

		while (!in.eof() && offset++ < _offset)
		{
			in.read(&ch, 1);

			//if (/*ch == '\r' || */ch == '\n')
			if (ch == 13 && offset < _offset)
				line_number++;
		}

		return line_number;
	}

	int fileOffsetToLineNumberMem(const char* _pBuffer, size_t _size, size_t _file_offset)
	{
		char ch = 0;
		uint64_t offset = 0;
		int line_number = 1;

		while (offset < _size && offset < _file_offset)
		{
			ch = _pBuffer[offset];

			if (ch == '\r' || ch == '\n') {
				line_number++;
			}

			offset++;
		}

		return line_number;
	}
}
