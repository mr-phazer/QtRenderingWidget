#pragma once

#include <string>

namespace anim
{
	/// <summary>
	/// Structure to contain all possible values of "TW ANIM" (v5), v6, v7, v8
	/// </summary>
	struct AnimHeaderCommon
	{
		std::uint32_t dwVersion = 0;
		std::uint32_t dwUnknown = 0;
		float fFrameRate = 0.0f;

		std::string skeletonName = "";

		uint32_t dwBoneCount = 0;

		struct  SubHeaderV7
		{
			uint32_t flag_count = 0;;
			std::string strText = "";
		}
		subHeaderV7;

		float fEndTime = -1.0f;

		struct  SubHeaderV8 // DOES this exist?
		{
			uint32_t dwUnknown1 = 0;
			uint32_t dwUnknown2 = 0;
		}
	};
}