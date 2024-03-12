#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <GeneralTypes\Animation\AnimFrameCommon.h>
#include "BoneTable.h"
#include "FileHeader.h"

namespace anim_file
{
	struct AnimFile : IDataStructure
	{
		bool IsContentValid() override
		{
			return	(
				fileHeader.IsContentValid() ||
				!boneTable.bones.empty() ||
				!frames.empty()
				)
				? false : true;
		}

		AnimHeaderCommon fileHeader;
		BoneTable boneTable;
		std::vector<AnimFrameCommon> frames;
	};
}