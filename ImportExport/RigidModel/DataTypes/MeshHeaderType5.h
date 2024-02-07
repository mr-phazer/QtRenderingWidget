#pragma once

#include <DirectXMath.h>
#include <cstdint>

#include "..\..\Helpers\ByteStream.h"
#include "MeshEnumsConstants.h"

namespace rmv2
{
	// TODO this is a not a bit for bit data struct it is a COMMON...
	struct MeshHeader5Common
	{
		bool IsContentValid();
		static constexpr size_t GetSsize = 860;

		VertexFormatEnum VertexFormatId = VertexFormatEnum(0);

		char szMeshName[32] = "NOT_SET_MESH_NAME";
		char szTextureDirectory[256] = "NOT_SET_TEXT_DIR";

		// !!NOT!! part of file format
		char szFilters[256] = { 0 };

		uint8_t paddingByte0;
		uint8_t paddingByte1;
		//int bone_index = -1; // not in format;

		// 3x4 indentity matrices. They are stored like CA stores the bind pose matrices for skeletons.
		// So the last row is implicit = 0, 0, 0, 1 (you have to put it in yourself)
		struct Rmv2HeaderTransform
		{
			DirectX::XMFLOAT3 vPivot = { 0,0,0 };

			DirectX::XMFLOAT3X4 matrix1 = CAIdentityMatrix3x4;
			DirectX::XMFLOAT3X4 matrix2 = CAIdentityMatrix3x4;
			DirectX::XMFLOAT3X4 matrix3 = CAIdentityMatrix3x4;
		}
		transformation;

		int32_t iMatrixIndex = -1;
		int32_t iParentMatrixIndex = -1;

		//I scale the whole model so it's probably not a single block!
		uint32_t dwAttachmentPointCount = 0;	// Count of weapon attachment points blocks
		uint32_t dwTextureCount = 0;			// Count of texture info blocks in group

		struct MaterialParams {
			uint32_t stringParamCount = ~0;
			uint32_t floatParamCount = ~0;
			uint32_t intParamCount = ~0;
			uint32_t float4ParamCount = ~0;
		}
		materialParams;

		uint8_t szPadding[124] = { 0 };
	};

}; // namespace rmv2