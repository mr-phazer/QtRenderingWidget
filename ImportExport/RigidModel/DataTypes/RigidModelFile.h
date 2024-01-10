#pragma once

#include "FileHeader.h"
#include "LodHeader.h"
#include "MeshHeader_V3.h"

namespace rmv2 {
	struct RigidModelFileCommon
	{
		file_header::FileHeaderCommon fileHeader;
		std::vector<lod_header::LODHeaderCommon> lodHeaders;
	};

	struct MeshChunkCommon
	{
		mesh_header::MeshHeaderV3Common meshHeader;


	};
} // namespace rmv2