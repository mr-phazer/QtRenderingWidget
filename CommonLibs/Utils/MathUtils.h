#pragma once

#include <string>
#include <DirectXMath.h>

namespace utils {
	
	using float2 = DirectX::XMFLOAT2;
	using float3 = DirectX::XMFLOAT3;
	using float4 = DirectX::XMFLOAT4;
	using float4x4 = DirectX::XMFLOAT4X4;
	using float3x3 = DirectX::XMFLOAT3X3;

	float ToDegrees(float radians);
	float ToRadians(float degrees);

	/// <summary>
	/// Return true at a certain pronanality
	/// if input is "0.7" there is a 70% change that it will return true
	/// </summary>
	/// <param name="propbabality">ratio [0;1]</param>
	/// <returns></returns>
	bool ProbablityFunction(float ratio);

	template <typename CONST_BUF_DATA_TYPE>
	CONST_BUF_DATA_TYPE Sign(CONST_BUF_DATA_TYPE in);

	/// <summary>
	///  Implemation of 'Polynomial rolling hash function'
	/// from
	/// https://cp-algorithms.com/string/string-hashing.html#calculation-of-the-hash-of-a-string 		
	/// </summary>
	/// <param name="s">string to be hased</param>
	/// <returns>64 bit hash</returns>
	static int64_t compute_string_hash(const std::wstring& s);
}
