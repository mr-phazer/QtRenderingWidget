#pragma once

#include <DirectXMath.h>
#include <ImportExport\GeneralTypes\IDataStructure.h>
#include "MeshEnumsConstants.h"

namespace rmv2
{
	// pure virtual getter methods returning references for all member variables
	struct MeshHeaderType3 : public IDataStructure
	{

		bool IsContentValid()  override
		{
			return
				(dwVertexCount <= 0xFFFF) &&
				(RigidMaterialId < 101);
		};


		// TODO: you COULD have such a thing, like oles IMaterial, tho it doesn't fit for this, as it is apparently the same for ALL rmv2s
  //      // Getters
		//virtual RigidMaterialEnum& RigidMaterialId() = 0;
		//virtual uint16_t& RenderFlags() = 0;
		//virtual uint32_t& MeshSectionSize() = 0;
		//virtual uint32_t& VertexOffset() = 0;
		//virtual uint32_t& VertexCount() = 0;
		//virtual uint32_t& IndexOffset() = 0;
		//virtual uint32_t& IndexCount() = 0;
		//virtual DirectX::XMFLOAT3& MinBB() = 0;
		//virtual DirectX::XMFLOAT3& MaxBB() = 0;
		//virtual char* LightingConstants() = 0;


		static constexpr size_t GetHeaderSize()
		{
			return sizeof(MeshHeaderType3);
		};

		size_t GetVertexSize() const
		{
			return static_cast<size_t>((dwVertexOffset - dwIndexOffset) / dwVertexCount);
		};

		RigidMaterialEnum RigidMaterialId = RigidMaterialEnum(0);	// Indicates the material of the group, some material make the group chunk larger, which merits further discovery
		uint16_t  wRenderFlags = 0;
		// number of bytes from the start of this header to the begining of the next group (starting at the header)
		uint32_t dwMeshSectionSize = 0;	// Size of the entire group (header + attachment blocks + texture blocks + vertices + indices)

		uint32_t dwVertexOffset = 0; // The combined size of all attachment blocks and all texture blocks in current group
		uint32_t dwVertexCount = 0;		// Count of vertices in group

		uint32_t dwIndexOffset = 0; // size of vertex data + attachment point table
		uint32_t dwIndexCount = 0;		// 	Number of indices// Absolute offset of next grouop = (sum of preverious group sizes) + current group size + LodHeaders[lod].offset

		DirectX::XMFLOAT3 vMinBB;
		DirectX::XMFLOAT3 vMaxBB;

		char  szLightingConstants[32] = ""; // almost always = "default_dry";
	};

} // namespace rmv2
