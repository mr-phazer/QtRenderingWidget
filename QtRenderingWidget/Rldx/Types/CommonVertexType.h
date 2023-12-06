#pragma once

#include <vector>


#include <DirectXMath.h>

#include <d3dcompiler.h>
#include <d3d11.h>

namespace Rldx
{
	namespace dx = DirectX;

	struct CommonVertex
	{
		CommonVertex(
			dx::XMFLOAT3 const& iposition,
			dx::XMFLOAT3 const& inormal,
			dx::XMFLOAT3 const& itangent,
			dx::XMFLOAT3 const& ibitangent,
			dx::XMFLOAT4 const& icolor,
			dx::XMFLOAT2 const& itextureCoordinate);;

		void SetVertexDataCommonVertex(
			dx::XMFLOAT3 const& iposition,
			dx::XMFLOAT3 const& inormal,
			dx::XMFLOAT3 const& itangent,
			dx::XMFLOAT3 const& ibitangent,
			dx::XMFLOAT4 const& icolor,
			dx::XMFLOAT2 const& itextureCoordinate);;

		dx::XMFLOAT4 position;
		dx::XMFLOAT3 normal;
		dx::XMFLOAT3 tangent;
		dx::XMFLOAT3 bitangent;
		dx::XMFLOAT2 textureCoordinate;
		dx::XMFLOAT4 color;
	};

}; // namespace DirectX






