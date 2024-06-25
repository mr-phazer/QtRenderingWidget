#include "..\Logging\Logging.h"
#include "COMerrors.h"
#include "tools.h"

// TODO: unused, but ought to be used for COM-errors, to geta meaningful error msg
std::wstring getComErrorWide(HRESULT hr)
{
	_com_error err(hr);
	LPCTSTR errMsg = err.ErrorMessage();
	return std::wstring(errMsg);
}

std::string GetComErrorMsgNarrow(HRESULT hr)
{
	_com_error err(hr);
	LPCTSTR errMsg = err.ErrorMessage();
	return NarrowStr(std::wstring(errMsg));
}

bool comAssert_Box(HRESULT hr, std::string _func, std::string _operation)
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func = WidenStr(_func);
		auto op = WidenStr(_operation);

		std::wostringstream ss;
		ss << std::hex << hr;
		std::wstring strHex = ss.str();

		auto msg = getComErrorWide(hr);

		MessageBox(
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
		);

		return false;
	};

	return true;
}

std::string GetComErrorMsgFullNarrow(HRESULT hr)
{
	std::ostringstream ssForHex;
	ssForHex << std::hex << hr;
	std::string strHex = ssForHex.str();

	auto strCOMError = "Error: " + strHex + ": " + GetComErrorMsgNarrow(hr);

	return strCOMError;
}

std::string GetComErrorFormated(HRESULT hr, const string& message)
{
	// TODO: ok to delete?
	//std::string s_str;
	//s_str.resize(128);

	//// -- add the hexadecimal error code
	//sprintf_s((char*)s_str.data(), 128, "Failure with HRESULT of 0x%08X", static_cast<unsigned int>(hr));

	//// -- add errorstd::string
	//s_str.resize(strlen(s_str.c_str())); // narrowstd::string from len = 128 down to actual size
	auto msg = GetComErrorMsgFullNarrow(hr);

	auto actionString = message.empty() ? "" : "Action: " + message + " : ";
	return actionString + msg + '\0';
}

bool comAssert_LogOnly(HRESULT hr, std::string _func, std::string _operation)
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func = WidenStr(_func);
		auto op = WidenStr(_operation);

		std::ostringstream ss;
		ss << std::hex << hr;
		std::string strHex = ss.str();


		std::stringstream stream;
		stream << std::hex << hr;
		std::string hrResult(stream.str());

		auto msg = getComErrorWide(hr);

		logging::LogActionError(": Direct3d Error " + std::string(hrResult) + ": " + strHex);


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
