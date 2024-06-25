#pragma once

#include <string>
#include <vector>
#include <comdef.h>
#include <DirectXMath.h>

//---------------------------------------------------------------------//
// Functions for String manipulation
// 
// There's only an implementation for std::wstring. String of type
// string can be converted with ToWString and back with ToString.
//---------------------------------------------------------------------//

namespace utils {

	std::wstring ToWString(const std::string& str);
	std::string ToString(const std::wstring& wstr);
	std::wstring F3ToWString(DirectX::XMFLOAT3 _v3);
	std::wstring F4ToWString(DirectX::XMFLOAT4 _v4);

	std::wstring decToHexW(int dec);

	wchar_t ToLower(wchar_t ch);
	wchar_t ToUpper(wchar_t ch);

	std::wstring ToLower(std::wstring s);
	std::wstring ToUpper(std::wstring s);
	std::string ToUpper(std::string s);

	template<typename T_it>
	void SequenceToLower(T_it begin, T_it end);

	template<typename T_it>
	void SequenceToUpper(T_it begin, T_it end);

	std::vector<std::wstring> DissolveStringBytDelimitersFast(const std::wstring& _strInput, const wchar_t chDelimiterChar);
	std::vector<std::wstring> DissolveStringBytDelimiters(const std::wstring& _path, const std::wstring& delimiters);

	template <typename Out>
	void split(const std::wstring& s, wchar_t delim, Out result);
	std::vector<std::wstring> split(const std::wstring& s, wchar_t delim);

	bool CompareNoCase(const std::wstring& str1, const std::wstring& str2);
	bool CompareExtension(const std::wstring& str1, const std::wstring& str2);

	std::wstring ToForwardSlash(const std::wstring& _strInput);
	std::wstring ToLowerAndBackSlash(const std::wstring& _strInput);
}