#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <vector>

#include <codecvt>
#include <cassert>
#include <string>
#include <string.h>

#include <algorithm>
#include <fstream>
#include <comdef.h>

#include "tools.h"
#include "..\Logging\Logging.h"
	


	static std::wstring getComErrorWide(HRESULT hr)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		return std::wstring(errMsg);
	}

	static std::string getComErrorNarrow(HRESULT hr)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		return NarrowStr(std::wstring(errMsg));
	}

static bool comAssert_Box(HRESULT hr,std::string _func = "",std::string _operation = "")
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func =WidenStr(_func);
		auto op =WidenStr(_operation);

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
};

static std::string GetCommErrorNarrow(HRESULT hr)
{
	std::ostringstream ssForHex;
	ssForHex << std::hex << hr;
	std::string strHex = ssForHex.str();

	auto strCOMError = "Error: " + strHex + ": " + getComErrorNarrow(hr);

	return strCOMError;
}


static bool comAssert_LogOnly(HRESULT hr,std::string _func = "",std::string _operation = "")
{
	bool bResult = SUCCEEDED(hr);

	if (!bResult)
	{
		auto func =WidenStr(_func);
		auto op =WidenStr(_operation);

		std::wostringstream ss;
		ss << std::hex << hr;
		std::wstring strHex = ss.str();


		std::stringstream stream;
		stream << std::hex << hr;
		std::string result(stream.str());

		auto msg = getComErrorWide(hr);

		LogActionError(": Direct3d Error " +std::string(result) + ": " + strHex);


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
};

class COMException : public std::exception
{
public:
	COMException(HRESULT hr) noexcept : result(hr) {}

	const char* what() const override
	{
		s_str.resize(128);

		// -- add the hexadecimal error code
		sprintf_s((char*)s_str.data(), 128, "Failure with HRESULT of 0x%08X", static_cast<unsigned int>(result));

		// -- add errorstd::string
		s_str.resize(strlen(s_str.c_str())); // narrowstd::string from len = 128 down to actual size
		auto msg = getComErrorNarrow(result);
		s_str += " : " + msg + '\0';

		return s_str.c_str();
	}

	HRESULT get_result() const noexcept { return result; }

private:
	HRESULT result;
	mutable std::string s_str;
};

};

#define COM_COND_THROW(HR) \
if (!SUCCEEDED(HR)) throw tools::COMException(HR); \

const auto _ws = tools::wstring_to_string;
const auto _sw = tools::string_to_wstring;

#define LOADING_FILE(path) (std::string("Loading: ") + std::string(path))

#define COM_ASSERT_BOX(HR, op) tools::comAssert_Box(HR, __FUNCTION__, op)

#define COM_ASSERT(HR) tools::comAssert_LogOnly(HR, __FUNCTION__)//\
//assert(SUCCEEDED(HR));\

#define COM_ASSERT_OP(HR, op) tools::comAssert_LogOnly(HR, __FUNCTION__, op)
//\
// assert(SUCCEEDED(HR));\

#define ComMessageBox(hr, _title, _op) \
		auto message = wstring(tools::string_to_wstring(__FUNCTION__) + L"\r\n") \
	+ wstring(L"Failed: " + wstring(_op) + _wstrPath + L"\r\n") \
		+ wstring(L"HRESULT: ") + std::to_wstring(hr) + L"\r\n" \
		+ wstring(L"String: ") + tools::getComError(hr) + L"\r\n"; \
	MessageBox(NULL, message.c_str(), _title, MB_OK | MB_ICONERROR);\


