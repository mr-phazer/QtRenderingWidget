#pragma once

#pragma once

#include <cstdint>

namespace anim_file
{
	struct FrameHeaderCommon
	{
		uint32_t translationCount = 0;
		uint32_t rotationCount = 0;
		uint32_t frameCount = 0;
	};
}
