#pragma once

#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>

namespace rldx
{
	using float2 = DirectX::XMFLOAT2;
	using float3 = DirectX::XMFLOAT3;
	using float4 = DirectX::XMFLOAT4;
	using uint = uint32_t;
	using dword = uint32_t;
	using qword = uint64_t;

	using float4x4 = DirectX::XMFLOAT4X4;
	using float3x3 = DirectX::XMFLOAT3X3;

	using float32 = float;
	using float64 = double;
	using float16 = uint16_t;
}