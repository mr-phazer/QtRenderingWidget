#pragma once

#include<vector>
#include "..\..\RigidModel\Types\Common\TextureElement.h"

namespace rmv2
{
	struct XMLMaterialData
	{
		void AddTexture(TextureTypeEnum textureType, const std::string& path) { textures.push_back({ textureType, path }); }

		std::vector<TextureElement> textures;
		std::wstring xmlMaterialFilePath;
	};
}