#pragma once


#include  <cwctype>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <vector>

#include <chrono>
#include <codecvt>
#include <string>
#include <string.h>

#include <algorithm>
#include <comdef.h>
#include <fstream>

//#include <locale>
#include <DirectXMath.h>
#include "Utils\StrUtils.h"

using namespace std;
using namespace utils;

namespace libtools
{
	static bool comAssert_Box(HRESULT hr, std::string _func = "", std::string _operation = "")
	{
		bool bResult = SUCCEEDED(hr);

		if (!bResult)
		{
			auto func = ToWString(_func);
			auto op = ToWString(_operation);

			std::wostringstream ss;
			ss << std::hex << hr;
			std::wstring strHex = ss.str();

			auto msg = GetComError(hr);

			MessageBox(
				NULL,
				(
					L"Operation:" + op + L"\n\r" +
					L"Function: " + func + L"\n\r" +
					L"Error Code: \n\r" +
					L"Hex: 0x" + strHex + L"\n\r" +
					to_wstring(hr) + L" :\n\r" +
					L"Error std::string: " + msg
				).c_str(),
				L"Critical Direct3d Error!",
				MB_OK | MB_ICONERROR
			);

			return false;
		};

		return true;
	};

	static bool comAssert_LogOnly(HRESULT hr, std::string _func = "", std::string _operation = "");;

	class COMException : public std::exception
	{
	public:
		COMException(HRESULT hr) noexcept : result(hr) {};

		const char* what() const override;

		HRESULT GetHrResult() const noexcept { return result; };

		operator HRESULT() { return result; };

	private:
		HRESULT result;

	};
};

#define COM_COND_THROW(HR) \
if (!SUCCEEDED(HR)) throw libtools::COMException(HR); \

#define LOADING_FILE(path) (std::string("Loading: ") + std::string(path))

#define COM_ASSERT_BOX(HR, op) libtools::comAssert_Box(HR, __FUNCTION__, op)

#define COM_ASSERT(HR) libtools::comAssert_LogOnly(HR, __FUNCTION__)
#define COM_ASSERT_OP(HR, op) libtools::comAssert_LogOnly(HR, __FUNCTION__, op)

#define ComMessageBox(hr, _title, _op) \
		auto message = std::wstring(ToWString(__FUNCTION__) + L"\r\n") \
	+ std::wstring(L"Failed: " + std::wstring(_op) + _wstrPath + L"\r\n") \
		+ std::wstring(L"HRESULT: ") + std::to_wstring(hr) + L"\r\n" \
		+ std::wstring(L"String: ") + libtools::getComError(hr) + L"\r\n"; \
	MessageBox(NULL, message.c_str(), _title, MB_OK | MB_ICONERROR);\


#define VERBOSE __FUNCTION__ ": " __FILE__ "(" + std::to_string(__LINE__) + ")"

namespace guard
{

	template <typename CONST_BUF_DATA_TYPE>
	static void ThrowIfOutOfBounds(const std::vector<CONST_BUF_DATA_TYPE>& inVector, size_t index, const std::string& strWhat = "")
	{
		if (inVector.size() <= index)
		{
			throw out_of_range(strWhat.c_str());
		};
	}
}


