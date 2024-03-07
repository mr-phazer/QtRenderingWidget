#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace anim_file
{
	/// <summary>
	/// TW ANIM bone table element data
	/// The frame generator uses these indexes to process node transform in the correct hierachy
	/// </summary>
	struct BoneData
	{
		int32_t id = 0;
		int32_t parent_id = 0;
		std::string strName = "empty";
	};

	/// <summary>
	/// TW ANIM Bone table
	/// Represents a skeleton in linear format, assumed nodes are sorted, to parent always come before its children
	/// </summary>
	struct BoneTable
	{
		std::vector<BoneData> bones;
	};
}
