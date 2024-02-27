#include "pch.h"
#include "StrUtils.h"

#include <algorithm>
#include <codecvt>
#include <cwctype>

namespace Utils {

	//---------------------------------------------------------------------//
	// Functions for string conversion
	//---------------------------------------------------------------------//

	std::string ToString(const std::wstring& wstr)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}

	std::wstring ToWString(const std::string& str)
	{
		using convert_typeX = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	//---------------------------------------------------------------------//
	// Functions for character manipulation
	//---------------------------------------------------------------------//

	inline wchar_t ToLower(wchar_t ch)
	{
		using namespace std;
		return static_cast<wchar_t>(std::towlower(static_cast<unsigned short>(ch)));
	}

	inline wchar_t ToUpper(wchar_t ch)
	{
		using namespace std;
		return static_cast<wchar_t>(std::towupper(static_cast<unsigned short>(ch)));
	}

	//---------------------------------------------------------------------//
	// Functions for string manipulation
	//---------------------------------------------------------------------//

	inline std::wstring ToLower(std::wstring s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			[](wchar_t c) {
				return std::towlower(c);
			}
		);

		return s;
	}

	inline std::wstring ToUpper(std::wstring s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			[](wchar_t c) {
				return std::towupper(c);
			}
		);

		return s;
	}

	//---------------------------------------------------------------------//
	// Functions for comparison
	//---------------------------------------------------------------------//

	inline bool CompareNoCase(const std::wstring& str1, const std::wstring& str2)
	{
		return ToLower(str1) == ToLower(str2);
	}

	//---------------------------------------------------------------------//
	// Functions for path (string) manipulation
	//---------------------------------------------------------------------//

	static std::wstring ToForwardSlash(const std::wstring& _strInput)
	{
		std::wstring tempString = _strInput;
		for (auto& ch : tempString)
		{
			if (ch == '\\' || ch == 92)
			{
				ch = '/';
			}
		}

		return tempString;
	}
}