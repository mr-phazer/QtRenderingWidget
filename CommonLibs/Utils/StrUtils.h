#pragma once

#include <string>

//---------------------------------------------------------------------//
// Functions for String manipulation
// 
// There's only an implementation for std::wstring. String of type
// string can be converted with ToWString and back with ToString.
//---------------------------------------------------------------------//

namespace utils {

	std::wstring ToWString(const std::string& str);
	std::string ToString(const std::wstring& wstr);

	inline wchar_t ToLower(wchar_t ch);
	inline wchar_t ToUpper(wchar_t ch);

	inline std::wstring ToLower(std::wstring s);
	inline std::wstring ToUpper(std::wstring s);

	inline bool CompareNoCase(const std::wstring& str1, const std::wstring& str2);
	inline bool CompareExtension(const std::wstring& str1, const std::wstring& str2);

	static std::wstring ToForwardSlash(const std::wstring& _strInput);
}