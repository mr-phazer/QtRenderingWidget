#include "pch.h"
#include "IOUtils.h"

namespace Utils {

	//---------------------------------------------------------------------//
	// Functions for file detection
	//---------------------------------------------------------------------//

	bool FileExist(const std::wstring& name)
	{
		struct _stat buffer {};
		return (_wstat(name.c_str(), &buffer) == 0);
	};

	static bool IsDiskFile(const std::wstring& _str)
	{
		if (_str.size() < 2)
			return false;

		return (_str[1] == L':');
	};
}