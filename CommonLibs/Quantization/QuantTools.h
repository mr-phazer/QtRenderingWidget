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


	inline static constexpr float GetUNormFloat(uint8_t fixedPoint)
	{
		return static_cast<float>(fixedPoint) / 256.0f;
	}

	inline static constexpr float GetSNormFloatFromByte(uint8_t fixedPoint)
	{
		return (static_cast<float>(fixedPoint) / 127.0f) - 1.f;
	}

	inline static constexpr float GetSNormFloatFromSINT8(int8_t fixedPoint)
	{
		return (static_cast<float>(fixedPoint) / 128.0f);
	}

	inline static constexpr float GetSNormFloatFromSignedInt16(int16_t fixedPoint)
	{
		return (static_cast<float>(fixedPoint) / (32767.0f));
	}

	inline static DirectX::XMFLOAT4 GetSNormFloat4FromByte4(DirectX::PackedVector::XMUBYTE4 vIn)
	{
		return
		{
			(static_cast<float>(vIn.x) / 127.0f) - 1.f,
			(static_cast<float>(vIn.y) / 127.0f) - 1.f,
			(static_cast<float>(vIn.z) / 127.0f) - 1.f,
			(static_cast<float>(vIn.w) / 127.0f) - 1.f
		};
	};

	inline static DirectX::XMFLOAT3 GetSNORMFloat3FromUByte4(DirectX::PackedVector::XMUBYTE4 vIn)
	{
		return
		{
			GetSNormFloatFromByte(vIn.x),
			GetSNormFloatFromByte(vIn.y),
			GetSNormFloatFromByte(vIn.z),
		};
	};

	inline static DirectX::XMFLOAT3 GetSNORMFloat3FromSINT84(DirectX::PackedVector::XMBYTE4 vIn)
	{
		return
		{
			GetSNormFloatFromSINT8(vIn.x),
			GetSNormFloatFromSINT8(vIn.y),
			GetSNormFloatFromSINT8(vIn.z),
		};
	};

	inline static DirectX::XMFLOAT3 GetSNORMFloat3FromSINT84(int8_t* p)
	{
		return GetSNORMFloat3FromSINT84(DirectX::PackedVector::XMBYTE4(p[0], p[1], p[2], 0));
	}

	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedByte4(DirectX::PackedVector::XMBYTE4 vIn)
	{
		return
		{
			GetSNormFloatFromSINT8(vIn.x),
			GetSNormFloatFromSINT8(vIn.y),
			GetSNormFloatFromSINT8(vIn.z),
			GetSNormFloatFromSINT8(vIn.w)
		};
	};

	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedByte4(int8_t* p)
	{
		return GetSNormFloat4FromSignedByte4(DirectX::PackedVector::XMBYTE4(p[0], p[1], p[2], p[3]));
	};

	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedWord4(DirectX::PackedVector::XMSHORT4 vIn)
	{
		return	{
			GetSNormFloatFromSignedInt16(vIn.x),
			GetSNormFloatFromSignedInt16(vIn.y),
			GetSNormFloatFromSignedInt16(vIn.z),
			GetSNormFloatFromSignedInt16(vIn.w),
		};
	};

	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedWord4(int16_t* p)
	{
		return GetSNormFloat4FromSignedWord4(DirectX::PackedVector::XMSHORT4(p[0], p[1], p[2], p[3]));
	};


	inline static DirectX::XMFLOAT4 GetUNORMFloat4FromUnsigned4(DirectX::PackedVector::XMBYTE4 vIn)
	{
		return
		{
			static_cast<float>(vIn.x) / 255.0f,
			static_cast<float>(vIn.y) / 255.0f,
			static_cast<float>(vIn.z) / 255.0f,
			static_cast<float>(vIn.w) / 255.0f
		};
	};

	inline static constexpr DirectX::XMFLOAT4 GetUNORMFloat4FromByte4(DirectX::PackedVector::XMUBYTE4 vIn)
	{
		return
		{
			static_cast<float>(vIn.x) / 255.0f,
			static_cast<float>(vIn.y) / 255.0f,
			static_cast<float>(vIn.z) / 255.0f,
			static_cast<float>(vIn.w) / 255.0f
		};
	};


	inline static DirectX::XMFLOAT4 GetFloat4FromHalf4(const DirectX::PackedVector::XMUSHORT4& in)
	{
		return {
			DirectX::PackedVector::XMConvertHalfToFloat(in.x),
			DirectX::PackedVector::XMConvertHalfToFloat(in.y),
			DirectX::PackedVector::XMConvertHalfToFloat(in.z),
			DirectX::PackedVector::XMConvertHalfToFloat(in.w)
		};
	}

	inline static DirectX::XMFLOAT2 GetFloat2FromHalf2(const DirectX::PackedVector::XMUSHORT2& in)
	{
		return {
			DirectX::PackedVector::XMConvertHalfToFloat(in.x),
			DirectX::PackedVector::XMConvertHalfToFloat(in.y),
		};
	}

	inline static float GetSNORMFloatFromByte(uint8_t in)
	{
		return (static_cast<float>(in) / 127.0f) - 1.f;
	};

	static float GetUNORMFloatFromByte(uint8_t b)
	{
		return static_cast<float>(b) / 255.0f;
	};
};