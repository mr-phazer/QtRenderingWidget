#pragma once

#include "..\..\..\Helpers\ByteStream.h"

namespace anim_file
{
	uint32_t GetTWAnimFileVersion(ByteStream& bytes);
}
