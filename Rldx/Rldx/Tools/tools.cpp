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
		std::string result(stream.str());

		auto msg = GetComError(hr);
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
	sprintf_s((char*)tempString.data(), fixedStringLength, "Failure with HRESULT of 0x%08X", static_cast<unsigned int>(result));

	// -- add error string
	tempString.resize(strlen(tempString.c_str())); // resize string to fit the error string
	auto comErrorMessage = GetComErrorNarrow(result);
	tempString += " : " + comErrorMessage + '\0';

	return tempString.c_str();
}
