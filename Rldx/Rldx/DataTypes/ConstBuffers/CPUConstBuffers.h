#pragma once

#include "CPU_PS_ConstBuffers.h"
#include "CPU_VS_ConstBuffers.h"



namespace rldx {

	struct VS_PerMesh_ConstBuffer
	{
		DirectX::XMFLOAT4X4 mWeaponWorld = sm::Matrix::Identity;
		DirectX::XMFLOAT4X4 mNodeWorld = sm::Matrix::Identity;
		DirectX::XMFLOAT3 pivot = { 0,0,0 };
		int32_t bone_index = -1;

		uint32_t has_alpha = 0;
		int32_t padding1;
		int32_t padding2;
		int32_t padding3;
	};

	struct PS_PerMesh_ConstBuffer
	{
		float reflectivity = 1.0;
		float ambientLight = 1.0;
		float reserved1 = 0.0f;
		float reserved2 = 0.0f;

		uint32_t bDoFactionColoring = 1;
		uint32_t bDoTinting = 1;
		uint32_t alpha_used = 1; // default at ON
		uint32_t mask_index = 0;

		float decal_u = 1.0;
		float decal_v = 1.0;
		float u_div = 1.0;
		float v_div = 1.0;

		DirectX::XMFLOAT4 decal_uv_rect = { 0,0,0,0 }; // TODO: is this value decals, and is it for statics only?

		sm::Color color_no_tex = { 155., 155., 155., 1. };

		sm::Color mesh_faction_color1 = { 0.71f, 0.16f, 0.17f, 1 };
		sm::Color mesh_faction_color2 = { 0.31f, 0.12f, 0.92f, 1 };
		sm::Color mesh_faction_color3 = { 0,0,1,1 };

		sm::Color tint_color1 = { 1,0,0,1 };
		sm::Color tint_color2 = { 0,1,0,1 };
		sm::Color tint_color3 = { 0,0,1,1 };

		DirectX::XMUINT4 puiMaskIndices = { 0,1,2,3 };

		uint32_t PisTextureSRGB[64] = { 0 };
	};

	struct VS_PerScene_ConstantBuffer
	{
		// TODO: maybe move into camera struct?
		DirectX::XMFLOAT3 eyePosition = { 0.f ,0.f, 0.f };
		uint32_t reserved1 = 0;

		float3 eyeDirection;
		int32_t radiance = 1;

		DirectX::XMFLOAT4X4 world = sm::Matrix::Identity;
		DirectX::XMFLOAT4X4 view = sm::Matrix::Identity;
		DirectX::XMFLOAT4X4 projection = sm::Matrix::Identity;

		//PS_DirectionalLight_ConstBuffer light[1];
	};



	/// <summary>
	/// data for 1 skeleton, used for skinning
	/// Simplest possiible implementation, static arrays of matrices
	/// </summary>
	struct VS_PerMeshConstBuffer_Skeleton
	{
		static constexpr size_t MAX_BONES = 256;

		uint32_t boneCount; // Count count, so GPU skinning don't have to run through all the empty matrices?
		uint32_t pading1; // padding for 16 byte alignment, maybe use this for something later
		uint32_t pading2; // padding for 16 byte alignment, maybe use this for something later
		uint32_t pading3; // padding for 16 byte alignment, maybe use this for something later

		sm::Matrix boneTransform[MAX_BONES];
		sm::Matrix inverseBindPoseMatrices[MAX_BONES];
	};


}; // namespace rldx 