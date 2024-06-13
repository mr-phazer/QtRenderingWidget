#pragma once

#include <string>

namespace utils {

	//---------------------------------------------------------------------//
	// Functions for file detection
	//---------------------------------------------------------------------//

	bool FileExist(const std::wstring& name);
	static bool IsDiskFile(const std::wstring& str);
}