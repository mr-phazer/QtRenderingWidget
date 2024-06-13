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
