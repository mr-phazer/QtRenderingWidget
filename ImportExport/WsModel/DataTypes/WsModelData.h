#pragma once

#include <vector>
#include "..\..\RigidModel\DataTypes\TextureElement.h"
#include "..\..\XmMaterial\DataTypes\XMLMaterialData.h"

namespace rmv2 {	

	struct WsModelData
	{
		std::wstring geometryPath;
		std::vector<std::vector<XMLMaterialData>> xmlMateriData;
	};
}