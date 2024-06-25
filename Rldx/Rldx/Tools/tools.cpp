#include <string>
#include "tools.h"

using namespace std;


bool libtools::comAssert_LogOnly(HRESULT hr, string _func, string _operation)
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func = ToWString(_func);
		auto op = ToWString(_operation);

		std::wostringstream ss;
		ss << std::hex << hr;
		std::wstring strHex = ss.str();


		std::stringstream stream;
		stream << std::hex << hr;
		std::string hrResult(stream.str());

		auto msg = getComErrorWide(hr);

		//		LogActionError(_func + " " + _operation + string(": Direct3d Error " + string(hrResult) + ": ") + std::wstring_to_string(strHex));


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


inline const char* libtools::COMException::what() const
{
	const size_t fixedStringLength = 128;

	std::string tempString;
	tempString.resize(fixedStringLength);

	// -- add the hexadecimal error code, 
	sprintf_s((char*)tempString.data(), fixedStringLength, "Failure with HRESULT of 0x%08X", static_cast<unsigned int>(hrResult));

	// -- add error string
	tempString.resize(strlen(tempString.c_str())); // resize string to fit the error string
	auto comErrorMessage = GetComErrorMsgNarrow(hrResult);
	tempString += " : " + comErrorMessage + '\0';

	return tempString.c_str();
}
