#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

struct quant_tools {

	template <typename UINT_TYPE>
	static inline constexpr float GetUNormFloatFromUINT(UINT_TYPE integerfixedPoint);

	template <typename UINT_TYPE>
	static inline constexpr float TGetSNormFloatFromInt(UINT_TYPE integerfixedPoint);

	template <typename CONST_BUF_DATA_TYPE>
	inline static constexpr DirectX::XMFLOAT3 TGetSNORMFloat3(const CONST_BUF_DATA_TYPE& vIn);

	inline static constexpr float GetUNormFloat(uint8_t fixedPoint);
	inline static constexpr float GetSNormFloatFromByte(uint8_t fixedPoint);
	inline static constexpr float GetSNormFloatFromSINT8(int8_t fixedPoint);
	inline static constexpr float GetSNormFloatFromSignedInt16(int16_t fixedPoint);
	inline static DirectX::XMFLOAT4 GetSNormFloat4FromByte4(DirectX::PackedVector::XMUBYTE4 vIn);
	inline static DirectX::XMFLOAT3 GetSNORMFloat3FromUByte4(DirectX::PackedVector::XMUBYTE4 vIn);
	inline static DirectX::XMFLOAT3 GetSNORMFloat3FromSINT84(DirectX::PackedVector::XMBYTE4 vIn);
	inline static DirectX::XMFLOAT3 GetSNORMFloat3FromSINT84(int8_t* p);
	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedByte4(DirectX::PackedVector::XMBYTE4 vIn);
	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedByte4(int8_t* p);
	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedWord4(DirectX::PackedVector::XMSHORT4 vIn);
	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedWord4(int16_t* p);
	inline static DirectX::XMFLOAT4 GetUNORMFloat4FromUnsigned4(DirectX::PackedVector::XMBYTE4 vIn);
	inline static constexpr DirectX::XMFLOAT4 GetUNORMFloat4FromByte4(DirectX::PackedVector::XMUBYTE4 vIn);
	inline static DirectX::XMFLOAT4 GetFloat4FromHalf4(const DirectX::PackedVector::XMUSHORT4& in);
	inline static DirectX::XMFLOAT2 GetFloat2FromHalf2(const DirectX::PackedVector::XMUSHORT2& in);
	inline static float GetSNORMFloatFromByte(uint8_t in);
	static float GetUNORMFloatFromByte(uint8_t b);
};