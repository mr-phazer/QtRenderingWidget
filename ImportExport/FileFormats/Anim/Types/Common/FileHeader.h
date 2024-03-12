#pragma once

#include <cstdint>
#include <string>


#include "..\..\ImportExport\GeneralTypes\IDataStructure.h"

namespace anim_file
{
	enum AnimVersionEnum : uint32_t
	{
		ANIM_VERSION_5 = 5,
		ANIM_VERSION_6 = 6,
		ANIM_VERSION_7 = 7,
		ANIM_VERSION_8 = 8
	};

	/// <summary>
	/// Structure to contain all possible values of "TW ANIM" (v5), v6, v7, v8
	/// </summary>
	struct AnimHeaderCommon : IDataStructure
	{
		bool IsContentValid()
		{
			if (
				dwVersion > 8 ||
				fFrameRate == NAN ||
				fFrameRate == 0.0f ||
				skeletonName.empty() ||
				dwBoneCount == 0

				)
				return false;

			return true;
		}



		std::uint32_t dwVersion = 0;
		std::uint32_t dwUnknown = 0;
		float fFrameRate = 0.0f;

		std::string skeletonName = "";

		uint32_t dwBoneCount = 0;

		struct  SubHeaderV7
		{
			uint32_t flagCount = 0;;
			std::string strText = "";
		}
		subHeader_V7;

		float fEndTime = -1.0f;

		struct SubHeaderV8 // DOES this exist?
		{
			uint32_t dwUnknown1 = 0;
			uint32_t dwUnknown2 = 0;
		}
		subHeader_v8;
	};
}