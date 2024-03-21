#include <string>
#include "tools.h"

using namespace std;

int libtools::fileOffsetToLineNumber(const std::wstring& _strFile, uint64_t _offset)
{
	ifstream in(_strFile, ios::binary);
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

int libtools::fileOffsetToLineNumberMem(const char* _pBuffer, size_t _size, size_t _file_offset)
{
	/*ifstream in(_strFile, ios::binary);
	if (!in.is_open())
		return 0;*/

	char ch = 0;
	uint64_t offset = 0;
	int line_number = 1;

	while (offset < _size && offset < _file_offset)
	{
		ch = _pBuffer[offset];

		if (ch == '\r' || ch == '\n')
			//if (ch == '\r' && offset < _file_offset)
			line_number++;

		offset++;
	}

	return line_number;
}

bool libtools::comAssert_LogOnly(HRESULT hr, string _func, string _operation)
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func = string_to_wstring(_func);
		auto op = string_to_wstring(_operation);

		std::wostringstream ss;
		ss << std::hex << hr;
		std::wstring strHex = ss.str();


		std::stringstream stream;
		stream << std::hex << hr;
		std::string result(stream.str());

		auto msg = getComErrorWide(hr);

		//		LogActionError(_func + " " + _operation + string(": Direct3d Error " + string(result) + ": ") + std::wstring_to_string(strHex));


				/*	MessageBox(
				NULL,
				(
				wstring(L"Operation:") + op + L"\n\r" +
				L"Function: " +
				wstring(func) + L"\n\r" +
				wstring(L"Error Code: ") + L"\n\r" +
				wstring(L"Hex: ") + L"0x" + strHex + L"\n\r" +
				to_wstring(hr) + L" :" + L"\n\r" +
				wstring(L"Error String: " + msg)).c_str(),
				L"Critical Direct3d Error!",
				MB_OK | MB_ICONERROR
				);*/

		return false;
	};

	return true;
}

//TODO: check that this actually workjs
bool libtools::ProbablityFunction(float ratio)
{
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX + 1)) < ratio;
}

bool libtools::DoesFileExist(const std::wstring& name)
{
	struct _stat buffer {};
	return (_wstat(name.c_str(), &buffer) == 0);
};

std::wstring libtools::string_to_wstring(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string libtools::wstring_to_string(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::wstring libtools::convert_string(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string libtools::convert_string(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::wstring libtools::GetExePath()
{
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(NULL, path, MAX_PATH);
	std::wstring exePath = libtools::GetDirFromFullPath(path);

	return exePath;
}

std::string libtools::GetDirFromFullPath(const std::string& _str)
{
	string strTemp = _str;

	if (strTemp == "")
		return "";

	size_t i = 0;
	int i1 = 0;
	size_t index = strTemp.size();
	size_t cut = 0;;

	while (strTemp[index] != 92 && strTemp[index] != '/' && index > 0)
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

std::wstring libtools::GetDirFromFullPath(const std::wstring& _str)
{
	wstring strTemp = _str;

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

std::string libtools::GetFileExtension(const std::string& _str)
{
	std::string out = _str;

	auto dotPosition = out.find('.');
	if (dotPosition == std::string::npos) // no dot, return unchanged
		return out;

	out.erase(0, dotPosition);

	return out;


	if (_str == "")
		return "";

	bool bFounDot = false;
	size_t len = _str.length();
	size_t index = len - 1;

	string strTemp = "";

	// move the index back to where the extension starts
	while (_str[index] != '.' && index > 0)
	{
		index--;
	}

	if (index <= 0)
		return "";

	strTemp = &_str[index + 1];

	return strTemp;
}

std::wstring libtools::GetFileExtension(const std::wstring& _str)
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

	wstring strTemp = L"";

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

std::wstring libtools::RemoveExtension(const std::wstring& _str)
{
	if (_str == L"")
		return L"";

	bool bFounDot = false;
	size_t len = _str.length();
	size_t index = len - 1;

	wstring strTemp = _str;

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

std::string libtools::RemoveExtension(const std::string& _str)
{
	if (_str == "")
		return "";

	bool bFounDot = false;
	size_t len = _str.length();
	size_t index = len - 1;

	string strTemp = _str;

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

std::string libtools::GetFileFromFullPath(const std::string& _strInput)
{
	if (_strInput.empty())
		return "";

	const char* _csz = _strInput.c_str();
	size_t len = strlen(_csz);

	if (len == 0)
	{
		return "";
	}

	size_t index = len - 1;

	// move the index back to where the extension starts
	while (_csz[index] != '\\' && _csz[index] != '/' && index > 0)
	{
		index--;
	}

	if (index < 0)
		return "";

	if (index == 0) // if there is no path in front of file name½
		return &_csz[index];

	return &_csz[index + 1];
}

std::wstring libtools::GetFileFromFullPath(const std::wstring& _strInput)
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

string libtools::lower(const std::string& _strInput)
{
	std::string strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

	return strOut;
}

string libtools::upper(const std::string& _strInput)
{
	std::string strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);

	return strOut;
}

wstring libtools::lower(const std::wstring& _strInput)
{
	std::wstring strOut;
	strOut.resize(_strInput.size(), L' ');

	for (size_t i = 0; i < _strInput.size(); i++)
	{
		strOut[i] = tolower(_strInput[i]);
	}

	return strOut;
}

string libtools::getLowerAndBackSlash(const std::string& _strInput)
{
	std::string strOut = _strInput;

	for (auto& it : strOut)
	{
		it = tolower(it);
		if (it == '/')
			it = '\\';
	}

	//boneTransform(strOut.begin(), strOut.end(), strOut.begin(), tolower);

	return strOut;
}

size_t libtools::getFileSize(const std::string& strFileName)
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
	}*/

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


size_t libtools::GetFileSize(const std::wstring& strFileName)
{
	struct _stat stat_buf {};
	int rc = _wstat(strFileName.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : 0;
}

inline const char* libtools::COMException::what() const
{
	const size_t fixedStringLength = 128;

	std::string tempString;
	tempString.resize(fixedStringLength);

	// -- add the hexadecimal error code, 
	sprintf_s((char*)tempString.data(), fixedStringLength, "Failure with HRESULT of 0x%08X", static_cast<unsigned int>(result));

	// -- add error string
	tempString.resize(strlen(tempString.c_str())); // resize string to fit the error string
	auto comErrorMessage = getComErrorNarrow(result);
	tempString += " : " + comErrorMessage + '\0';

	return tempString.c_str();
}
