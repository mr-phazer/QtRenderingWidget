#pragma once

#include<vector>
#include "..\..\RigidModel\DataTypes\TextureElement.h"

namespace rmv2
{
	struct XmlMaterialData
	{
		// TODO: more info?
		std::vector <TextureElement> textures;
	};	
}