#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "AnimFrameCommon.h"
#include "BoneTable.h"
#include "FileHeader.h"

namespace anim_file
{
	struct TwAnimFile
	{
		AnimHeaderCommon fileHeader;
		BoneTable boneTable;

		std::vector<AnimFrameCommon> frames;
	};
}