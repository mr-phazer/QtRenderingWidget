#pragma once

#pragma once

#include <cstdint>

namespace anim_file
{
	struct FrameHeaderCommon
	{
		uint32_t translationCount;
		uint32_t rotationCount;
		uint32_t frameCount;
	};
}
