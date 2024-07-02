#include <algorithm>
#include <codecvt>
#include <cwctype>
#include <sstream>
#include "IOUtils.h"
#include "StrUtils.h"

namespace utils {

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

	std::wstring F3ToWString(DirectX::XMFLOAT3 _v3)
	{
		return
			L"(" +
			std::to_wstring(_v3.x) + L", " +
			std::to_wstring(_v3.y) + L", " +
			std::to_wstring(_v3.z) + L" ) ";
	}

	std::wstring F4ToWString(DirectX::XMFLOAT4 _v4)
	{
		return
			L"(" +
			std::to_wstring(_v4.x) + L", " +
			std::to_wstring(_v4.y) + L", " +
			std::to_wstring(_v4.z) + L", " +
			std::to_wstring(_v4.w) + L" ) ";
	}

	std::wstring decToHexW(int dec)
	{
		std::wostringstream ss;
		ss << std::hex << dec;

		return ss.str();
	}

	//---------------------------------------------------------------------//
	// Functions for character manipulation
	//---------------------------------------------------------------------//

	wchar_t ToLower(wchar_t ch)
	{
		using namespace std;
		return static_cast<wchar_t>(std::towlower(static_cast<unsigned short>(ch)));
	}

	wchar_t ToUpper(wchar_t ch)
	{
		using namespace std;
		return static_cast<wchar_t>(std::towupper(static_cast<unsigned short>(ch)));
	}

	//---------------------------------------------------------------------//
	// Functions for string manipulation
	//---------------------------------------------------------------------//

	std::wstring ToLower(std::wstring s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
					   [](wchar_t c) {
						   return std::towlower(c);
					   }
		);

		return s;
	}

	std::wstring ToUpper(std::wstring s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
					   [](wchar_t c) {
						   return std::towupper(c);
					   }
		);

		return s;
	}

	std::string ToUpper(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
					   [](unsigned char c) { return std::toupper(c); } // correct
		);

		return s;
	}

	template<typename T_it>
	void SequenceToLower(T_it begin, T_it end)
	{
		// Convert to upper: clear the '32' bit, 0x20 in hex. And with the
		// inverted bit std::string (~).
		for (auto it = begin; it != end; ++it)
			*it |= 0x20;
	}

	template<typename T_it>
	void SequenceToUpper(T_it begin, T_it end)
	{
		// Convert to upper: clear the '32' bit, 0x20 in hex. And with the
		// inverted bit std::string (~).
		for (auto it = begin; it != end; ++it)
			*it &= ~0x20;
	}

	std::vector<std::wstring> DissolveStringBytDelimitersFast(const std::wstring& _strInput, const wchar_t chDelimiterChar = L' ')
	{
		std::wistringstream ss(_strInput);
		std::wstring strTempDest;

		std::vector<std::wstring> vecStringOut;
		while (std::getline(ss, strTempDest, chDelimiterChar))
		{
			vecStringOut.push_back(strTempDest);
		}

		return vecStringOut;
	}

	std::vector<std::wstring> DissolveStringBytDelimiters(const std::wstring& _path, const std::wstring& delimiters)
	{
		// lamda for checking if any of chars
		auto isDelimiter = [&](wchar_t ch) -> bool
			{
				for (const auto& itCh : delimiters)
				{
					if (tolower(itCh) == ch)
						return true;
				}

				return false;
			};

		std::vector<std::wstring> vecDissolvedPath;
		size_t str_index = 0;
		while (str_index < _path.size())
		{
			std::wstring strTemp = L"";
			while (!isDelimiter(_path[str_index]) && str_index < _path.size())
			{
				strTemp += _path[str_index];
				str_index++;
			}
			vecDissolvedPath.push_back(strTemp);
			str_index++;
		}

		return vecDissolvedPath;
	}

	template <typename Out>
	void split(const std::wstring& s, wchar_t delim, Out result) {
		std::wistringstream iss(s);
		std::wstring item;
		while (std::getline(iss, item, delim)) {
			*result++ = item;
		}
	}

	std::vector<std::wstring> split(const std::wstring& s, wchar_t delim) {
		std::vector<std::wstring> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	//---------------------------------------------------------------------//
	// Functions for comparison
	//---------------------------------------------------------------------//

	bool CompareNoCase(const std::wstring& str1, const std::wstring& str2)
	{
		return ToLower(str1) == ToLower(str2);
	}

	bool CompareExtension(const std::wstring& str1, const std::wstring& str2)
	{
		return CompareNoCase(GetFileExtension(str1), GetFileExtension(str2));
	}

	//---------------------------------------------------------------------//
	// Functions for path (string) manipulation
	//---------------------------------------------------------------------//

	std::wstring ToForwardSlash(const std::wstring& _strInput)
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

	std::wstring ToLowerAndBackSlash(const std::wstring& _strInput)
	{
		std::wstring strOut = _strInput;

		for (auto& it : strOut)
		{
			it = ToLower(it);
			if (it == '/')
				it = '\\';
		}

		return strOut;
	}
}