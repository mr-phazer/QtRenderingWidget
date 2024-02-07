#pragma once

#include "XMLMaterialData.h"

namespace rmv2 {

	struct WsModelData
	{
		std::wstring geometryPath;
		std::vector<std::vector<std::wstring>> xmlMateriFilePaths;
	};
}