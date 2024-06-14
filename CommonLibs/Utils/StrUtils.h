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
	static std::wstring ToWString(DirectX::XMFLOAT3 _v3);
	static std::wstring ToWString(DirectX::XMFLOAT4 _v4);

	static std::wstring decToHexW(int dec);

	inline wchar_t ToLower(wchar_t ch);
	inline wchar_t ToUpper(wchar_t ch);

	inline std::wstring ToLower(std::wstring s);
	inline std::wstring ToUpper(std::wstring s);
	inline std::string ToUpper(std::string s);

	template<typename T_it>
	static inline void SequenceToLower(T_it begin, T_it end);

	template<typename T_it>
	static inline void SequenceToUpper(T_it begin, T_it end);

	static std::vector<std::wstring> DissolveStringBytDelimitersFast(const std::wstring& _strInput, const wchar_t chDelimiterChar);
	static std::vector<std::wstring> DissolveStringBytDelimiters(const std::wstring& _path, const std::wstring& delimiters);

	template <typename Out>
	void split(const std::wstring& s, wchar_t delim, Out result);
	static std::vector<std::wstring> split(const std::wstring& s, wchar_t delim);

	inline bool CompareNoCase(const std::wstring& str1, const std::wstring& str2);
	inline bool CompareExtension(const std::wstring& str1, const std::wstring& str2);

	static std::wstring ToForwardSlash(const std::wstring& _strInput);
	static std::wstring ToLowerAndBackSlash(const std::wstring& _strInput);

	std::wstring GetComError(HRESULT hrResult);
	std::string GetComErrorNarrow(HRESULT hrResult);
}