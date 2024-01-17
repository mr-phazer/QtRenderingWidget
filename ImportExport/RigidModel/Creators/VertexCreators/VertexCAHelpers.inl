#pragma once

struct CAVertexHelpers
{
	static DirectX::XMFLOAT4 DoVersion8Precision(const DirectX::XMFLOAT4& position)
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

};