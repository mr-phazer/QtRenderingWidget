#pragma once

#include<vector>
#include "..\..\RigidModel\DataTypes\TextureElement.h"

namespace rmv2
{
	struct XMLMaterialData
	{
		void AddTexture(TextureTypeEnum textureType, const std::string& path) { textures.push_back({ textureType, path }); }		

		std::wstring xmlMaterialFilePath;
		std::vector<TextureElement> textures;
	};
}