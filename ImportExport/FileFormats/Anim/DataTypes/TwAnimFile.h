#pragma once

#include <cstdint>
#include <string>
#include <vector>


#include "..\DataTypes\AnimHeaderCommon.h"
#include "..\DataTypes\BoneTable.h"

namespace anim_file
{
	struct TwAnimFile
	{
		AnimHeaderCommon fileHeader;
		BoneTable boneTable;

	};
}