#pragma once

#include <DirectXMath.h>

namespace rldx
{
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
		DirectX::XMUINT4 boneIndices = { 0,0,0,0 };
		DirectX::XMFLOAT4 weights = {1.0f, 0, 0, 0};
	};
}
