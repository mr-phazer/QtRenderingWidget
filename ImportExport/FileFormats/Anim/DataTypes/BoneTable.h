#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace anim_file
{
	/// <summary>
	/// TW ANIM bone table element data
	/// </summary>
	struct BoneData
	{
		int32_t id = 0;
		int32_t parent_id = 0;
		std::string strName = "empty";
	};

	/// <summary>
	/// TW ANIM Bone table
	/// </summary>
	struct BoneTable
	{
		std::vector<BoneData> bones;
	};
}
