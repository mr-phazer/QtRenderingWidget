#pragma once

#include <DirectXMath.h>

struct MeshHeaderType5
{
	static constexpr size_t size = 860;
	RigidModelV2::VertexFormatInfo::EVertexFormat VertexFormatId = RigidModelV2::VertexFormatInfo::EVertexFormat::eERROR_NOT_SET;

	char szMeshName[32] = "mesh_no_name";		// First believed to be zero padded 32 char, but is not present at all in certain "obscure" RMV2 "sub formats"
	/
	//wchar_t szwGroupName[32];	// utf-16 version of abovestd::string, seems only to be used in RMV2 version 5, (see File header)

	// !!NOT!! part of file format_

	std::string szTextureDirectory = std::string(256, '\0');		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory
	std::wstring wszTextureDirectory = std::wstring(256, L'\0');		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory
	//wchar_t wszTextureDirecory[256];		// 256 uint8_ts[0 - Paddedstd::string] - TexturesDirectory

	// !!NOT!! part of file format
	char szFilters[256] = { 0 };
	char wszFilters[256] = { 0 };

	uint16_t wUK_2;
	int bone_index = -1; // not in format;
	DirectX::XMFLOAT3 vPivot = { 0,0,0 };

	// 3x4 indentity matrices. They are stored like CA stores the bind pose matrices for skeletons.
	// So the last row is implicit = 0, 0, 0, 1 (you have to put it in yourself)
	struct _Transfomation
	{
		DirectX::XMFLOAT3X4 matrix1 =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0
		};

		DirectX::XMFLOAT3X4 matrix2 =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0
		};
		DirectX::XMFLOAT3X4 matrix3 =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0
		};;
	} oTransformation;

	int32_t iMatrixIndex = -1;
	int32_t iParentMatrixIndex = -1;

	//I scale the whole model so it's probably not a single block!
	uint32_t dwAttachmentPointCount = 0;	// Count of weapon attachment points blocks
	uint32_t dwTextureCount = 0;			// Count of texture info blocks in group

	struct _values {
		uint32_t stringParamCount = 0;
		uint32_t floatParamCount = 0;
		uint32_t intParamCount = 0;
		uint32_t float4ParamCount = 0;
	} oMaterialValues;

	uint8_t szPadding[124] = { 0 };
}

