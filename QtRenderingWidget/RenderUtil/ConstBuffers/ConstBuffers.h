#pragma once

#include <DirectXMath.h>
#include "..\..\..\DirectXTK\Inc\SimpleMath.h"
#include "..\ConstBuffers\ConstBuffers.h"

namespace Rldx {

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
		float ambientlight = 1.0;
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


};