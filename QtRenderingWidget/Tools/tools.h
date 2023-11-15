#pragma once



#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <vector>

#include <codecvt>
#include <string>
#include <string.h>
#include <chrono>

#include <algorithm>
#include <fstream>
#include <comdef.h>

//#include <locale>
#include <DirectXMath.h>

//#include "..\SystemLib\Console\QConsole.h"


using namespace std;


static string toUpper(const std::string& _strInput)
{
	std::string strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);

	return strOut;
}

template <typename T>
T Sign(T in)
{
	return (abs(in) / in);
}

static wstring toUpper(const std::wstring& _strInput)
{
	std::wstring strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::toupper);

	return strOut;
}

static string toLower(const std::string& _strInput)
{
	std::string strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

	return strOut;
}

static wstring toLower(const std::wstring& _strInput)
{
	std::wstring strOut = _strInput;

	std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

	return strOut;
}

namespace std
{
	//static string tolower(const char* _szIn)
	//{
	//	string strTemp = _szIn;
	//	return tolower(strTemp);
	//}

	//static wstring tolower(const wchar_t* _wszIn)
	//{
	//	wstring wstrTemp = _wszIn;
	//	return tolower(wstrTemp);
	//}

	static string tolower(const std::string& _strInput)
	{
		std::string strOut = _strInput;

		std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

		return strOut;
	}

	static string toupper(const std::string& _strInput)
	{
		std::string strOut = _strInput;

		std::transform(strOut.begin(), strOut.end(), strOut.begin(), ::tolower);

		return strOut;
	}

	static wstring tolower(const std::wstring& _strInput)
	{
		std::wstring strOut;
		strOut.resize(_strInput.size(), L' ');

		for (size_t i = 0; i < _strInput.size(); i++)
		{
			strOut[i] = tolower(_strInput[i]);
		}

		return strOut;
	}

	static wstring toupper(const std::wstring& _strInput)
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

namespace tools
{

	/// <summary>
	/// Uses the CPUs high resolution clock, to count time intervals
	/// </summary>
	class SystemClock
	{
		typedef std::chrono::high_resolution_clock Time;
	public:
		SystemClock()
		{
			ResetLocalTime();
		}

		/// <summary>
		/// Resets the local timer to "now"
		/// </summary>
		void ResetLocalTime()
		{
			m_startTime = std::chrono::high_resolution_clock::now();
		}

		/// <summary>
		///  Get "local" time, 
		/// </summary>
		/// <returns>"state time" - "now" </returns>
		double GetLocalTime()
		{
			auto timeElapsed = std::chrono::high_resolution_clock::now();

			auto value = std::chrono::duration<float, std::chrono::seconds::period>(timeElapsed - m_startTime);

			float retValue = value.count();

			return retValue;
		}

	private:
		std::chrono::steady_clock::time_point m_startTime;
	};

	static bool isDiskFile(const string& _str)
	{
		if (_str.size() < 2)
			return false; 

		return (_str[1] == ':');
	};

	static bool isDiskFile(const wstring& _str)
	{
		if (_str.size() < 2)
			return false;


		return (_str[1] == L':');
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

	static inline bool doesFileExist(const std::wstring & _wstrPath)
	{
		std::ifstream inFile(_wstrPath, std::ios::binary);
		if (!inFile.is_open())
		{
			return false;
		}
		inFile.close();

		return true;
	}

	static uint64_t ___getFileSize(const std::wstring & _wstrPath)
	{
		std::ifstream inFile(_wstrPath, std::ios::binary);
		if (!inFile.is_open())
		{
			return 0;
		}

		inFile.seekg(0, ios_base::end);

		uint64_t offsetEOF = inFile.tellg();
		inFile.close();

		return offsetEOF+1;
	}


	template <typename STRING_TYPE>
	static bool string_compare(const STRING_TYPE& str1, const STRING_TYPE& str2)
	{
		// if they are not the same length, they cannot be equal
		if (str1.length() != str2.length())
		{
			return false;
		}

		// strings are same length, so we can use either length
		for (size_t i = 0; i < str1.length(); i++)
		{
			if (str1[i] != str2[i]) // if one character is not equal, the string are not equal,
			{
				return false;
			}
		}

		// made it through the loop, meaning string are equal
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

		// strings are same length, so we can use either length
		for (size_t i = 0; i < str1.length(); i++)
		{
			if (std::toupper(str1[i]) != std::toupper(str2[i])) // if one character is not equal, the string are not equal,
			{
				return false;
			}
		}

		// made it through the loop, meaning string are equal
		return true;
	}


	static string decToHex(int dec)
	{
		std::ostringstream ss;
		ss << std::hex << dec;
		
		return ss.str();
	}

	static wstring decToHexW(int dec)
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
		// inverted bit string (~).
		for (auto it = begin; it != end; ++it)
			*it &= ~0x20;
	}

	template<typename T_it>
	static inline void SequenceToLowerCase(T_it begin, T_it end)
	{
		// Convert to upper: clear the '32' bit, 0x20 in hex. And with the
		// inverted bit string (~).
		for (auto it = begin; it != end; ++it)
			*it |= 0x20;
	}

	//	template<typename T_it>
	static inline std::string _ToLowerCase(const std::string& in)
	{
		// Convert to upper: clear the '32' bit, 0x20 in hex. And with the
		// inverted bit string (~).

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

	template <class T>
	static bool compare(T _1, T _2);

	template <>
	static bool compare(string _1, string _2);

	std::string GetDirFromFullPath(const std::string& _str);

	std::wstring GetDirFromFullPath(const std::wstring& _str);
	std::wstring GetDirFromFullPath(const std::wstring& _str);

	std::string GetExtensionFromFullPath(const std::string& _str);
	std::wstring GetExtensionFromFullPath(const std::wstring& _str);

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
	size_t getFileSize(const std::wstring& strFileName);

	static inline size_t getInputFileLength(std::ifstream& is)
	{
		is.seekg(0, std::ios_base::end);
		std::size_t size = is.tellg();
		is.seekg(0, std::ios_base::beg);
		return size;
	}


	static string toForwardSlash(const std::string& _strInput)
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

	static wstring toForwardSlash(const std::wstring& _strInput)
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

	

	

	

	template<class T>
	bool compare(T _1, T _2)
	{
		return (_1 == _2);
	}

	template<>
	bool compare(string _1, string _2)
	{
		auto s1 = lower(_1);
		auto s2 = lower(_2);

		return (s1 == s2);
	}

	int fileOffsetToLineNumber(const wstring& _strFile, uint64_t _offset);

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

	static bool comAssert_Box(HRESULT hr, string _func = "", string _operation = "")
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
					std::wstring(L"Error String: " + msg)).c_str(),
				L"Critical Direct3d Error!",
				MB_OK | MB_ICONERROR
			);

			return false;
		};

		return true;
	};

	static bool comAssert_LogOnly(HRESULT hr, string _func = "", string _operation = "");;

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
if (!SUCCEEDED(HR)) throw tools::COMException(HR); \

const auto _ws = tools::wstring_to_string;
const auto _sw = tools::string_to_wstring;

#define LOADING_FILE(path) (std::string("Loading: ") + std::string(path))

#define COM_ASSERT_BOX(HR, op) tools::comAssert_Box(HR, __FUNCTION__, op)

#define COM_ASSERT(HR) tools::comAssert_LogOnly(HR, __FUNCTION__)
#define COM_ASSERT_OP(HR, op) tools::comAssert_LogOnly(HR, __FUNCTION__, op)

#define ComMessageBox(hr, _title, _op) \
		auto message = wstring(tools::string_to_wstring(__FUNCTION__) + L"\r\n") \
	+ wstring(L"Failed: " + wstring(_op) + _wstrPath + L"\r\n") \
		+ wstring(L"HRESULT: ") + std::to_wstring(hr) + L"\r\n" \
		+ wstring(L"String: ") + tools::getComError(hr) + L"\r\n"; \
	MessageBox(NULL, message.c_str(), _title, MB_OK | MB_ICONERROR);\


const auto WidenStr = tools::string_to_wstring;
const auto NarrowStr = tools::wstring_to_string;

const auto widen = tools::string_to_wstring;
const auto narrow = tools::wstring_to_string;

const auto wtos = tools::string_to_wstring;
const auto stow = tools::wstring_to_string;

#define VERBOSE __FUNCTION__ ": " __FILE__ "(" + std::to_string(__LINE__) + ")"

namespace guard
{

	template <typename T>
	static void ThrowIfOutOfBounds(const std::vector<T>& inVector, size_t index, 
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



