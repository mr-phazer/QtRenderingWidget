#pragma once

#include "FileHeader.h"
#include "LodHeader.h"

namespace rmv2 {
	struct RigidModelFileCommon
	{
		file_header::FileHeaderCommon fileHeader;
		std::vector<lod_header::LODHeaderCommon> lodHeaders;
	};
} // namespace rmv2