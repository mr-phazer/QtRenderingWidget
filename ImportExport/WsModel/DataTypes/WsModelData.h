#pragma once

#include <vector>
#include "..\..\XmMaterial\DataTypes\XMLMaterialData.h"

namespace rmv2 {	

	struct WsModelData
	{
		void AddXmlMaterial(XMLMaterialData& input, size_t lodIndex, size_t partIndex)
		{
			// resize 2d vectors as needed
			if (lodIndex >= xmlMateriData.size()) {
				xmlMateriData.resize(lodIndex + 1);
			}

			if (partIndex >= xmlMateriData[lodIndex].size()) {
				xmlMateriData[lodIndex].resize(partIndex + 1);
			}

			xmlMateriData[lodIndex][partIndex] = input;
		}


		std::wstring geometryPath;
		std::vector<std::vector<XMLMaterialData>> xmlMateriData;
	};
}