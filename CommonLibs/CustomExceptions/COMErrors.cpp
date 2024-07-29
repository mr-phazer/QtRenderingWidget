#include "COMerrors.h"

#include <CommonLibs\Logger\Logger.h>
#include <CommonLibs\Utils\StrUtils.h>

using namespace utils;

// TODO: unused, but ought to be used for COM-errors, to geta meaningful error msg
std::wstring GetComErrorW(HRESULT hr)
{
	_com_error err(hr);
	std::wstring errorMesssage = err.ErrorMessage();

	return errorMesssage;
}

std::string GetComError(HRESULT hr)
{
	return ToString(GetComErrorW(hr));
}

std::string GetComErrorFull(HRESULT hr)
{
	std::string strHex = ToString(decToHexW(hr));

	auto strCOMError = "Error: " + strHex + ": " + GetComError(hr);

	return strCOMError;
}

std::string GetComErrorFormated(HRESULT hr, const std::string& message)
{
	// TODO: ok to delete?
	//std::string s_str;
	//s_str.resize(128);

	//// -- add the hexadecimal error code
	//sprintf_s((char*)s_str.data(), 128, "Failure with HRESULT of 0x%08X", static_cast<unsigned int>(hr));

	//// -- add errorstd::string
	//s_str.resize(strlen(s_str.c_str())); // narrowstd::string from len = 128 down to actual size
	auto msg = GetComErrorFull(hr);

	auto actionString = message.empty() ? "" : "Action: " + message + " : ";
	return actionString + msg + '\0';
}

bool comAssert_Box(HRESULT hr, std::string _func, std::string _operation)
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func = ToWString(_func);
		auto op = ToWString(_operation);

		std::wstring strHex = decToHexW(hr);

		auto msg = GetComErrorW(hr);

		MessageBox(
			NULL,
			(
				L"Operation:" + op + L"\n\r" +
				L"Function: " +
				func + L"\n\r" +
				L"Error Code: " + L"\n\r" +
				L"Hex: " + L"0x" + strHex + L"\n\r" +
				std::to_wstring(hr) + L" :" + L"\n\r" +
				L"Errorstd::string: " + msg).c_str(),
			L"Critical Direct3d Error!",
			MB_OK | MB_ICONERROR
		);

		return false;
	};

	return true;
}

bool comAssert_LogOnly(HRESULT hr, std::string _func, std::string _operation)
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func = ToWString(_func);
		auto op = ToWString(_operation);

		std::wstring strHex = decToHexW(hr);

		auto msg = GetComErrorW(hr);

		using namespace logging;
		Logger::LogActionError(L": Direct3d Error: " + strHex);


		// TODO: ok to delete?
		/*	MessageBox(
		NULL,
		(
		wstring(L"Operation:") + op + L"\n\r" +
		L"Function: " +
		wstring(func) + L"\n\r" +
		wstring(L"Error Code: ") + L"\n\r" +
		wstring(L"Hex: ") + L"0x" + strHex + L"\n\r" +
		to_wstring(hr) + L" :" + L"\n\r" +
		wstring(L"Errorstd::string: " + msg)).c_str(),
		L"Critical Direct3d Error!",
		MB_OK | MB_ICONERROR
		);*/

		return false;
	};

	return true;
}
