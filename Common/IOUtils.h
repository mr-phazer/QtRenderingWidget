#pragma once

#include <string>

namespace Utils {

	//---------------------------------------------------------------------//
	// Functions for file detection
	//---------------------------------------------------------------------//

	bool FileExist(const std::wstring& name);
	static bool IsDiskFile(const std::wstring& str);
}