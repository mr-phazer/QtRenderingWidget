#pragma once

#include <DirectXMath.h>

#include "..\..\Helpers\ByteStream.h"
#include "MeshEnums.h"

namespace rmv2
{
	namespace mesh_header
	{
		struct MeshHeader_V3_Common
		{
			static constexpr size_t GetSize()
			{
				return 80;
				//return sizeof(MeshHeaderV3Common);
			};

			RigidMaterialEnum RigidMaterialId = RigidMaterialEnum::NOT_SET;		// Indicates the material of the group, some material make the group chunk larger, which merits further discovery
			uint16_t  wRenderFlags = 0;
			// number of bytes from the start of this header to the begining of the next group (starting at the header)
			uint32_t dwMeshSectionSize = 0;	// Size of the entire group (header + attachment blocks + texture blocks + vertices + indices)

			uint32_t dwVertexOffset = 0; // The combined size of all attachment blocks and all texture blocks in current group
			uint32_t dwVertexCount = 0;		// Count of vertices in group

			uint32_t dwIndexOffset = 0; // size of vertex data + attachment point table
			uint32_t dwIndexCount = 0;		// 	Number of indices// Absolute offset of next grouop = (sum of preverious group sizes) + current group size + LodHeaders[lod].offset

			DirectX::XMFLOAT3 vMinBB;
			DirectX::XMFLOAT3 vMaxBB;

			char  szLightingConstants[32] = "default_dry";
		};

		class IMeshHeader_V3_Creator
		{
		public:
			virtual MeshHeader_V3_Common Create(ByteStream& bytes) = 0;
		};
	}

} // namespace rmv2
