#pragma once

#include <cstdint>
#include <string>
#include <vector>


#include "..\DataTypes\BoneTable.h"
#include "..\DataTypes\FileHeader.h"

namespace anim_file
{
	struct TwAnimFile
	{
		AnimHeaderCommon fileHeader;
		BoneTable boneTable;

	};
}