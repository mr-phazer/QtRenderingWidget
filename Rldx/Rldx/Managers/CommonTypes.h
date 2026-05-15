#pragma once

#include <vector>
#include <string>

namespace rldx
{
	typedef void (*AssetFetchCallbackWrapper) (std::vector<std::wstring>* filesToFetch,	std::vector<std::vector<unsigned char>>* outBinFiles);
}