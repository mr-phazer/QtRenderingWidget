#pragma once

#include <DirectXMath.h>
#include <SimpleMath.h>

namespace Rldx {

	using float2 = DirectX::XMFLOAT2;
	using float3 = DirectX::XMFLOAT3;
	using float4 = DirectX::XMFLOAT4;
	using uint = uint32_t;

	using float4x4 = DirectX::XMFLOAT4X4;
	using float3x3 = DirectX::XMFLOAT3X3;

	struct PS_LightData_ConstBuffer
	{
		DirectX::XMFLOAT3 direction;
		float radiance;		
		DirectX::XMFLOAT4 color;
	
	}; // 32 bytes

	struct CommonVertex
	{
		DirectX::XMFLOAT4 position = { 0,0,0,0 };
		DirectX::XMFLOAT2 textureCoordinate = { 0,0 };;
		DirectX::XMFLOAT2 textureCoordinate2 = { 0,0 };;
		DirectX::XMFLOAT3 normal = { 0,0,0 };
		DirectX::XMFLOAT3 tangent = { 0,0,0 };
		DirectX::XMFLOAT3 bitangent = { 0,0,0 };

		DirectX::XMFLOAT4 color = { 1,0,0,1 };

		// bone indices
		DirectX::XMUINT4 boneIndices;
		DirectX::XMFLOAT4 weights;
	};

	struct VS_MeshConstantBuffer
	{
		DirectX::XMFLOAT4X4 mWorld = sm::Matrix::Identity;
		DirectX::XMFLOAT3 pivot = { 0,0,0 };
		int32_t has_alpha = 0;

		// for animation
		uint32_t bone_count = 256;
		float reserved211;
		float reserved222;
		float reserved232;
				
		DirectX::XMFLOAT4X4 tranforms[256] = {  }; // TODO: maybe only fill the skeletons that are to be used
		DirectX::XMFLOAT4X4 inverse[256] = {  };
	};

	struct PS_MeshConstantBuffer
	{
		float reflectivity = 1.0;
		float ambientLight = 1.0;
		float reserved1;
		float reserved2;

		uint32_t bDoFactionColoring = 1;
		uint32_t bDoTinting = 1;
		uint32_t alpha_used = 0;
		uint32_t mask_index = 0;

		float decal_u = 1.0;
		float decal_v = 1.0;
		float u_div = 1.0;
		float v_div = 1.0;

		DirectX::XMFLOAT4 decal_uv_rect = { 0,0,0,0 }; // TODO: is this value decals, and is it for statics only?

		sm::Color color_no_tex = { 155., 155., 155., 1. };

		sm::Color mesh_faction_color2 = { 1,1,1,1 };
		sm::Color mesh_faction_color3 = { 1,1,1,1 };
		sm::Color mesh_faction_color1 = { 1,1,1,1 };

		sm::Color tint_color1 = { 1,1,1,1 };
		sm::Color tint_color2 = { 1,1,1,1 };
		sm::Color tint_color3 = { 1,1,1,1 };

		DirectX::XMUINT4 puiMaskIndices = { 0, 1, 2, 0 };

		uint32_t PisTextureSRGB[64];
	};

	struct VS_PerScene_ConstantBuffer
	{
		// TODO: maybe move into camera struct?
		DirectX::XMFLOAT3 eyePosition = { 0.5,0.1,0.1 };
		uint32_t reserved1 = 0;

		float3 eyeDirection;
		int32_t radiance = 1.0f;

		DirectX::XMFLOAT4X4 world = sm::Matrix::Identity;
		DirectX::XMFLOAT4X4 view = sm::Matrix::Identity;
		DirectX::XMFLOAT4X4 projection = sm::Matrix::Identity;

		PS_LightData_ConstBuffer light[1];
	};

}; // namespace Rldx 