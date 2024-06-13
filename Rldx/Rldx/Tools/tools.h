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

using namespace std;;

//#include "..\SystemLib\Console\QConsole.h"

inline char char_toupper(char ch)
{
	return static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
}

inline wchar_t wchar_toupper(wchar_t ch)
{
	return static_cast<wchar_t>(std::towupper(static_cast<unsigned short>(ch)));
}

inline char char_tolower(char ch)
{
	return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

inline wchar_t wchar_tolower(wchar_t ch)
{
	return static_cast<wchar_t>(std::towlower(static_cast<unsigned short>(ch)));
}


inline std::string toUpper(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
				   [](unsigned char c) { return std::toupper(c); } // correct
	);

	return s;
}

inline std::wstring toUpper(std::wstring s)
{
	std::transform(s.begin(), s.end(), s.begin(),
				   [](wchar_t c) { return std::towupper(c); } // correct
	);

	return s;
}

inline std::string toLower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
				   [](unsigned char c) { return char_tolower(c); } // correct
	);

	return s;
}

inline std::wstring toLower(std::wstring s)
{
	std::transform(s.begin(), s.end(), s.begin(),
				   [](wchar_t c) { return std::towlower(c); } // correct
	);

	return s;
}

inline  bool CompareNoCase(const std::wstring& str1, const std::wstring& str2)
{
	return toLower(str1) == toLower(str2);
}

inline  bool CompareNoCase(const std::string& str1, const std::string& str2)
{
	return toLower(str1) == toLower(str2);
}



/*inline std::wstring toUpper_WRONG(const std::wstring& _strInput){
	std::wstring strOut = _strInput;

	std::boneTransform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);

	return strOut;
}

inline std::string toUpper(const std::string& _strInput)
{
	std::string strOut = _strInput;

	std::boneTransform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);

	return strOut;
}*/

template <typename CONST_BUF_DATA_TYPE>
CONST_BUF_DATA_TYPE Sign(CONST_BUF_DATA_TYPE in)
{
	return (abs(in) / in);
}


//static std::string toLower(const std::string& _strInput)
//{
//	std::string strOut = _strInput;
//
//	std::boneTransform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);
//
//	return strOut;
//}
//
//static std::wstring toLower(const std::wstring& _strInput)
//{
//	std::wstring strOut = _strInput;
//
//	std::boneTransform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);
//
//	return strOut;
//}

namespace std
{
	//static std::string tolower(const char* _szIn)
	//{
	//	string strTemp = _szIn;
	//	return tolower(strTemp);
	//}

	//static std::wstring tolower(const wchar_t* _wszIn)
	//{
	//	wstring wstrTemp = _wszIn;
	//	return tolower(wstrTemp);
	//}

	static std::string tolower(const std::string& _strInput)
	{
		std::string strOut = _strInput;

		std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

		return strOut;
	}

	static std::string toupper(const std::string& _strInput)
	{
		std::string strOut = _strInput;

		std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

		return strOut;
	}

	static std::wstring tolower(const std::wstring& _strInput)
	{
		std::wstring strOut;
		strOut.resize(_strInput.size(), L' ');

		for (size_t i = 0; i < _strInput.size(); i++)
		{
			strOut[i] = tolower(_strInput[i]);
		}

		return strOut;
	}

	static std::wstring toupper(const std::wstring& _strInput)
	{
		std::wstring strOut = _strInput;

		std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

		return strOut;
	}
}

namespace std
{
	static std::string  to_string(::DirectX::XMFLOAT3 _v3)
	{
		return
			"(" +
			std::to_string(_v3.x) + ", " +
			std::to_string(_v3.y) + ", " +
			std::to_string(_v3.z) + " ) ";
	}

	static std::string  to_string(::DirectX::XMFLOAT4 _v4)
	{
		return
			"(" +
			std::to_string(_v4.x) + ", " +
			std::to_string(_v4.y) + ", " +
			std::to_string(_v4.z) + ", " +
			std::to_string(_v4.w) + " ) ";
	}
}

static float asin_f(float x)
{

}


namespace libtools
{
	constexpr float ToDegrees(float radians) { return radians * (180.0f / DirectX::XM_PI); }
	constexpr float ToRadians(float degrees) { return degrees * (DirectX::XM_PI / 180.0f); }


	/// <summary>
	/// Return true at a certain pronanality
	/// if input is "0.7" there is a 70% change that it will return true
	/// </summary>
	/// <param name="propbabality">ratio [0;1]</param>
	/// <returns></returns>
	bool ProbablityFunction(float propbabalityRatio);


	bool DoesFileExist(const std::wstring& name);


	static bool IsDiskFile(const std::string& _str)
	{
		if (_str.size() < 2)
			return false;

		return (_str[1] == ':');
	};

	/// <summary>
	///  Implemation of 'Polynomial rolling hash function'
	/// from
	/// https://cp-algorithms.com/string/string-hashing.html#calculation-of-the-hash-of-a-string 		
	/// </summary>
	/// <param name="s">string to be hased</param>
	/// <returns>64 bit hash</returns>
	static int64_t compute_string_hash(string const& s) {

		const int p = 31;
		const int m = static_cast<int>(1e9 + 9);
		int64_t hash_value = 0;
		int64_t p_pow = 1;
		for (char c : s) {
			hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
			p_pow = (p_pow * p) % m;
		}
		return hash_value;
	}

	static inline bool doesFileExist(const std::wstring& _wstrPath)
	{
		std::ifstream inFile(_wstrPath, std::ios::binary);
		if (!inFile.is_open())
		{
			return false;
		}
		inFile.close();

		return true;
	}

	static uint64_t ___getFileSize(const std::wstring& _wstrPath)
	{
		std::ifstream inFile(_wstrPath, std::ios::binary);
		if (!inFile.is_open())
		{
			return 0;
		}

		inFile.seekg(0, ios_base::end);

		uint64_t offsetEOF = inFile.tellg();
		inFile.close();

		return offsetEOF + 1;
	}


	template <typename STRING_TYPE>
	static bool STRING_TYPEe(const STRING_TYPE& str1, const STRING_TYPE& str2)
	{
		// if they are not the same length, they cannot be equal
		if (str1.length() != str2.length())
		{
			return false;
		}

		// std::strings are same length, so we can use either length
		for (size_t i = 0; i < str1.length(); i++)
		{
			if (str1[i] != str2[i]) // if one character is not equal, the std::string are not equal,
			{
				return false;
			}
		}

		// made it through the loop, meaning std::string are equal
		return true;
	}

	template <typename STRING_TYPE>
	static bool string_compare_nocase(const STRING_TYPE& str1, const STRING_TYPE& str2)
	{
		// if they are not the same length, they cannot be equal
		if (str1.length() != str2.length())
		{
			return false;
		}

		// std::strings are same length, so we can use either length
		for (size_t i = 0; i < str1.length(); i++)
		{
			if (std::toupper(str1[i]) != std::toupper(str2[i])) // if one character is not equal, the std::string are not equal,
			{
				return false;
			}
		}

		// made it through the loop, meaning std::string are equal
		return true;
	}


	static std::string decToHex(int dec)
	{
		std::ostringstream ss;
		ss << std::hex << dec;

		return ss.str();
	}

	static std::wstring decToHexW(int dec)
	{
		std::wostringstream ss;
		ss << std::hex << dec;

		return ss.str();
	}


	static vector<std::string> dissolveStringBytDelimiters_Fast(const std::string& _strInput, const char chDelimiterChar = ' ')
	{
		std::istringstream ss(_strInput);
		std::string strTempDest;

		vector<string> vecStringOut;
		while (std::getline(ss, strTempDest, chDelimiterChar))
		{
			vecStringOut.push_back(strTempDest);
		}

		return vecStringOut;
	}

	static vector<std::wstring> dissolveStringBytDelimiters(const std::wstring& _path, const std::wstring& delimiters)
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

		vector<wstring> vecDissolvedPath;
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

	static vector<std::string> dissolveStringBytDelimiters(const std::string& _path, const std::string& delimiters)
	{
		auto isDelimiter = [&](char ch) -> bool
			{
				for (const auto& itCh : delimiters)
				{
					if (tolower(itCh) == tolower(ch))
						return true;
				}

				return false;
			};

		vector<string> vecDissolvedPath;
		size_t str_index = 0;
		while (str_index < _path.size())
		{
			std::string strTemp = "";
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

	template<typename T_it>
	static inline void SequenceToUpperCase(T_it begin, T_it end)
	{
		// Convert to upper: clear the '32' bit, 0x20 in hex. And with the
		// inverted bit std::string (~).
		for (auto it = begin; it != end; ++it)
			*it &= ~0x20;
	}

	template<typename T_it>
	static inline void SequenceToLowerCase(T_it begin, T_it end)
	{
		// Convert to upper: clear the '32' bit, 0x20 in hex. And with the
		// inverted bit std::string (~).
		for (auto it = begin; it != end; ++it)
			*it |= 0x20;
	}

	//	template<typename T_it>
	static inline std::string _ToLowerCase(const std::string& in)
	{
		// Convert to upper: clear the '32' bit, 0x20 in hex. And with the
		// inverted bit std::string (~).

		string strTemp = "";
		strTemp.resize(in.size());
		auto itTemp = strTemp.begin();
		for (const auto& T_it : in)
		{
			*(itTemp++) = T_it | 0x20;
		}

		return strTemp;
	}

	template <typename Out>
	void split(const std::string& s, char delim, Out result) {
		std::istringstream iss(s);
		std::string item;
		while (std::getline(iss, item, delim)) {
			*result++ = item;
		}
	}

	static std::vector<std::string> split(const std::string& s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, std::back_inserter(elems));
		return elems;
	}

	extern std::wstring string_to_wstring(const std::string& str);
	extern std::string wstring_to_string(const std::wstring& wstr);

	extern std::wstring convert_string(const std::string& str);
	extern std::string convert_string(const std::wstring& wstr);



	template <class CONST_BUF_DATA_TYPE>
	static bool compare(CONST_BUF_DATA_TYPE _1, CONST_BUF_DATA_TYPE _2);

	template <>
	static bool compare(string _1, std::string _2);

	std::wstring GetExePath();


	std::string GetDirFromFullPath(const std::string& _str);

	std::wstring GetDirFromFullPath(const std::wstring& _str);
	std::wstring GetDirFromFullPath(const std::wstring& _str);

	std::string GetFileExtension(const std::string& _str);
	std::wstring GetFileExtension(const std::wstring& _str);

	std::wstring RemoveExtension(const std::wstring& _str);
	std::string RemoveExtension(const std::string& _str);

	std::wstring RemoveExtension(const std::wstring& _str);
	std::string RemoveExtension(const std::string& _str);

	std::string GetFileFromFullPath(const std::string& _strInput);
	std::wstring GetFileFromFullPath(const std::wstring& _strInput);

	std::string lower(const std::string& _strInput);
	std::string upper(const std::string& _strInput);

	std::wstring lower(const std::wstring& _strInput);
	std::wstring upper(const std::wstring& _strInput);

	std::string getLowerAndBackSlash(const std::string& _strInput);

	size_t getFileSize(const std::string& strFileName);
	size_t GetFileSize(const std::wstring& strFileName);

	static inline size_t getInputFileLength(std::ifstream& is)
	{
		is.seekg(0, std::ios_base::end);
		std::size_t size = is.tellg();
		is.seekg(0, std::ios_base::beg);
		return size;
	}


	static std::string toForwardSlash(const std::string& _strInput)
	{
		string tempString = _strInput;
		for (auto& ch : tempString)
		{
			if (ch == '\\' || ch == 92)
			{
				ch = '/';
			}
		}

		return tempString;
	}

	static std::wstring toForwardSlash(const std::wstring& _strInput)
	{
		wstring tempString = _strInput;
		for (auto& ch : tempString)
		{
			if (ch == '\\' || ch == 92)
			{
				ch = '/';
			}
		}

		return tempString;
	}







	template<class CONST_BUF_DATA_TYPE>
	bool compare(CONST_BUF_DATA_TYPE _1, CONST_BUF_DATA_TYPE _2)
	{
		return (_1 == _2);
	}

	template<>
	bool compare(string _1, std::string _2)
	{
		auto s1 = lower(_1);
		auto s2 = lower(_2);

		return (s1 == s2);
	}

	int fileOffsetToLineNumber(const std::wstring& _strFile, uint64_t _offset);

	int fileOffsetToLineNumberMem(const char*, size_t size, size_t _offset);




	static std::wstring getComErrorWide(HRESULT hrResult)
	{
		_com_error err(hrResult);
		std::wstring errorMesssage = err.ErrorMessage();

		return errorMesssage;
	}

	static std::string getComErrorNarrow(HRESULT hrResult)
	{
		_com_error err(hrResult);
		std::wstring errorMesssage = err.ErrorMessage();
		return wstring_to_string(errorMesssage);
	}

	static bool comAssert_Box(HRESULT hr, std::string _func = "", std::string _operation = "")
	{
		bool bResult = SUCCEEDED(hr);

		if (!bResult)
		{
			auto func = string_to_wstring(_func);
			auto op = string_to_wstring(_operation);

			std::wostringstream ss;
			ss << std::hex << hr;
			std::wstring strHex = ss.str();

			auto msg = getComErrorWide(hr);

			MessageBox(
				NULL,
				(
					std::wstring(L"Operation:") + op + L"\n\r" +
					L"Function: " +
					std::wstring(func) + L"\n\r" +
					std::wstring(L"Error Code: ") + L"\n\r" +
					std::wstring(L"Hex: ") + L"0x" + strHex + L"\n\r" +
					to_wstring(hr) + L" :" + L"\n\r" +
					std::wstring(L"Error std::string: " + msg)).c_str(),
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

const auto _ws = libtools::wstring_to_string;
const auto _sw = libtools::string_to_wstring;

#define LOADING_FILE(path) (std::string("Loading: ") + std::string(path))

#define COM_ASSERT_BOX(HR, op) libtools::comAssert_Box(HR, __FUNCTION__, op)

#define COM_ASSERT(HR) libtools::comAssert_LogOnly(HR, __FUNCTION__)
#define COM_ASSERT_OP(HR, op) libtools::comAssert_LogOnly(HR, __FUNCTION__, op)

#define ComMessageBox(hr, _title, _op) \
		auto message = std::wstring(libtools::string_to_wstring(__FUNCTION__) + L"\r\n") \
	+ std::wstring(L"Failed: " + std::wstring(_op) + _wstrPath + L"\r\n") \
		+ std::wstring(L"HRESULT: ") + std::to_wstring(hr) + L"\r\n" \
		+ std::wstring(L"String: ") + libtools::getComError(hr) + L"\r\n"; \
	MessageBox(NULL, message.c_str(), _title, MB_OK | MB_ICONERROR);\


const auto WidenStr = libtools::string_to_wstring;
const auto NarrowStr = libtools::wstring_to_string;

const auto widen = libtools::string_to_wstring;
const auto narrow = libtools::wstring_to_string;

const auto wtos = libtools::wstring_to_string;
const auto stow = libtools::string_to_wstring;

#define VERBOSE __FUNCTION__ ": " __FILE__ "(" + std::to_string(__LINE__) + ")"

namespace guard
{

	template <typename CONST_BUF_DATA_TYPE>
	static void ThrowIfOutOfBounds(const std::vector<CONST_BUF_DATA_TYPE>& inVector, size_t index,
								   const std::string& strWhat = ""
								   /*const std::string& a_callerFunc = __FUNCTION__,
								   const std::string& a_fileName = __FILE__,
								   const std::string& a_lineNumber = std::to_string(__LINE__)
								   */

	)
	{
		if (inVector.size() <= index)
		{
			//throw out_of_range((strWhat+ ": " + a_callerFunc + ": " + a_fileName + ": " + a_lineNumber).c_str());
			throw out_of_range(strWhat.c_str());
		};
	}
}


inline  bool CompareExtension(const std::string& str1, const std::string& str2)
{
	return CompareNoCase(libtools::GetFileExtension(str1), libtools::GetFileExtension(str2));
}

inline  bool CompareExtension(const std::wstring& str1, const std::wstring& str2)
{
	return CompareNoCase(libtools::GetFileExtension(str1), libtools::GetFileExtension(str2));
}


