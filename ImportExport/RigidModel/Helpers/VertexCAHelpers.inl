#pragma once

struct CAVertexHelpers
{
	static DirectX::XMFLOAT4 DoRMV2Version8PrecisionOp(const DirectX::XMFLOAT4& position)
	{
		return {
			position.x * std::max<float>(1.0f, position.w),
			position.y * std::max<float>(1.0f, position.w),
			position.z * std::max<float>(1.0f, position.w),
			1
		};
	}

	static DirectX::XMFLOAT3 SwapZY(const DirectX::XMFLOAT3& vector)
	{
		return DirectX::XMFLOAT3(vector.z, vector.y, vector.x);
	}

	static DirectX::XMFLOAT3 SwapZYAnd4To3(const DirectX::XMFLOAT4& in)
	{
		return DirectX::XMFLOAT3(in.z, in.y, in.x);
	}

};