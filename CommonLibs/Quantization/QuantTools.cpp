#include "QuantTools.h"

#include <type_traits>

template <typename UINT_TYPE>
static inline constexpr float quant_tools::GetUNormFloatFromUINT(UINT_TYPE integerfixedPoint)
{
	// check for supported types at compile-time
	static_assert(
		(std::is_same<UINT_TYPE, uint8_t>() ||
			std::is_same<UINT_TYPE, uint16_t>() ||
			std::is_same<UINT_TYPE, uint32_t>() ||
			std::is_same<UINT_TYPE, uint64_t>()), "Not a supported unsigned integer");

	size_t divider =
		(
			static_cast<size_t>(2U)
			<<
			(sizeof(UINT_TYPE) * 8U - 1)
			)
		- 1;

	float retUnormFloat = static_cast<float>(integerfixedPoint) / static_cast<float>(divider);

	return retUnormFloat;
}

// TODO: remove? Clean up!
//template <typename UINT_TYPE>
//static inline constexpr float TGetUNormFloatFromUInt(UINT_TYPE integerfixedPoint)
//{
//	//for all support types	
//	static_assert(
//		(std::is_same<UINT_TYPE, uint8_t> == true ||
//		 std::is_same<UINT_TYPE, uint16_t> == true ||
//		 std::is_same<UINT_TYPE, uint32_t> = true ||
//		 std::is_same<UINT_TYPE, uint64_t> == true)
//		, "Wrong Input Type!");

//	size_t divider = static_cast<size_t>(2U) << ((sizeof(UINT_TYPE) / 8U));

//	float retSMormFloat = (static_cast<float>(integerfixedPoint) / static_cast<float>(divider));

//	return retSMormFloat;
//}

template <typename UINT_TYPE>
static inline constexpr float quant_tools::TGetSNormFloatFromInt(UINT_TYPE integerfixedPoint)
{
	//for all support types	
	static_assert(
		(std::is_same<UINT_TYPE, int8_t>() ||
			std::is_same<UINT_TYPE, int16_t>() ||
			std::is_same<UINT_TYPE, int32_t>() ||
			std::is_same<UINT_TYPE, int64_t>())
		, "Wrong Input Type!");

	size_t divider = static_cast<size_t>(1U) << ((sizeof(UINT_TYPE) * 8U) - 1);

	float retSMormFloat = (static_cast<float>(integerfixedPoint) / static_cast<float>(divider));

	return retSMormFloat;
}

template <typename CONST_BUF_DATA_TYPE>
inline static constexpr DirectX::XMFLOAT3 quant_tools::TGetSNORMFloat3(const CONST_BUF_DATA_TYPE& vIn)
{
	return
	{
		(static_cast<float>(vIn.x) / 127.0f) - 1.f,
		(static_cast<float>(vIn.y) / 127.0f) - 1.f,
		(static_cast<float>(vIn.z) / 127.0f) - 1.f
	};
};


inline constexpr float quant_tools::GetUNormFloat(uint8_t fixedPoint)
{
	return static_cast<float>(fixedPoint) / 256.0f;
}

inline constexpr float quant_tools::GetSNormFloatFromByte(uint8_t fixedPoint)
{
	return (static_cast<float>(fixedPoint) / 127.0f) - 1.f;
}

inline constexpr float quant_tools::GetSNormFloatFromSINT8(int8_t fixedPoint)
{
	return (static_cast<float>(fixedPoint) / 128.0f);
}

inline constexpr float quant_tools::GetSNormFloatFromSignedInt16(int16_t fixedPoint)
{
	return (static_cast<float>(fixedPoint) / (32767.0f));
}

inline DirectX::XMFLOAT4 quant_tools::GetSNormFloat4FromByte4(DirectX::PackedVector::XMUBYTE4 vIn)
{
	return
	{
		(static_cast<float>(vIn.x) / 127.0f) - 1.f,
		(static_cast<float>(vIn.y) / 127.0f) - 1.f,
		(static_cast<float>(vIn.z) / 127.0f) - 1.f,
		(static_cast<float>(vIn.w) / 127.0f) - 1.f
	};
};

inline DirectX::XMFLOAT3 quant_tools::GetSNORMFloat3FromUByte4(DirectX::PackedVector::XMUBYTE4 vIn)
{
	return
	{
		GetSNormFloatFromByte(vIn.x),
		GetSNormFloatFromByte(vIn.y),
		GetSNormFloatFromByte(vIn.z),
	};
};

inline DirectX::XMFLOAT3 quant_tools::GetSNORMFloat3FromSINT84(DirectX::PackedVector::XMBYTE4 vIn)
{
	return
	{
		GetSNormFloatFromSINT8(vIn.x),
		GetSNormFloatFromSINT8(vIn.y),
		GetSNormFloatFromSINT8(vIn.z),
	};
};

inline DirectX::XMFLOAT3 quant_tools::GetSNORMFloat3FromSINT84(int8_t* p)
{
	return GetSNORMFloat3FromSINT84(DirectX::PackedVector::XMBYTE4(p[0], p[1], p[2], 0));
}

inline DirectX::XMFLOAT4 quant_tools::GetSNormFloat4FromSignedByte4(DirectX::PackedVector::XMBYTE4 vIn)
{
	return
	{
		GetSNormFloatFromSINT8(vIn.x),
		GetSNormFloatFromSINT8(vIn.y),
		GetSNormFloatFromSINT8(vIn.z),
		GetSNormFloatFromSINT8(vIn.w)
	};
};

inline DirectX::XMFLOAT4 quant_tools::GetSNormFloat4FromSignedByte4(int8_t* p)
{
	return GetSNormFloat4FromSignedByte4(DirectX::PackedVector::XMBYTE4(p[0], p[1], p[2], p[3]));
};

inline DirectX::XMFLOAT4 quant_tools::GetSNormFloat4FromSignedWord4(DirectX::PackedVector::XMSHORT4 vIn)
{
	return	{
		GetSNormFloatFromSignedInt16(vIn.x),
		GetSNormFloatFromSignedInt16(vIn.y),
		GetSNormFloatFromSignedInt16(vIn.z),
		GetSNormFloatFromSignedInt16(vIn.w),
	};
};

inline DirectX::XMFLOAT4 quant_tools::GetSNormFloat4FromSignedWord4(int16_t* p)
{
	return GetSNormFloat4FromSignedWord4(DirectX::PackedVector::XMSHORT4(p[0], p[1], p[2], p[3]));
};


inline DirectX::XMFLOAT4 quant_tools::GetUNORMFloat4FromUnsigned4(DirectX::PackedVector::XMBYTE4 vIn)
{
	return
	{
		static_cast<float>(vIn.x) / 255.0f,
		static_cast<float>(vIn.y) / 255.0f,
		static_cast<float>(vIn.z) / 255.0f,
		static_cast<float>(vIn.w) / 255.0f
	};
};

inline constexpr DirectX::XMFLOAT4 quant_tools::GetUNORMFloat4FromByte4(DirectX::PackedVector::XMUBYTE4 vIn)
{
	return
	{
		static_cast<float>(vIn.x) / 255.0f,
		static_cast<float>(vIn.y) / 255.0f,
		static_cast<float>(vIn.z) / 255.0f,
		static_cast<float>(vIn.w) / 255.0f
	};
};


inline DirectX::XMFLOAT4 quant_tools::GetFloat4FromHalf4(const DirectX::PackedVector::XMUSHORT4& in)
{
	return {
		DirectX::PackedVector::XMConvertHalfToFloat(in.x),
		DirectX::PackedVector::XMConvertHalfToFloat(in.y),
		DirectX::PackedVector::XMConvertHalfToFloat(in.z),
		DirectX::PackedVector::XMConvertHalfToFloat(in.w)
	};
}

inline DirectX::XMFLOAT2 quant_tools::GetFloat2FromHalf2(const DirectX::PackedVector::XMUSHORT2& in)
{
	return {
		DirectX::PackedVector::XMConvertHalfToFloat(in.x),
		DirectX::PackedVector::XMConvertHalfToFloat(in.y),
	};
}

inline float quant_tools::GetSNORMFloatFromByte(uint8_t in)
{
	return (static_cast<float>(in) / 127.0f) - 1.f;
};

static float GetUNORMFloatFromByte(uint8_t b)
{
	return static_cast<float>(b) / 255.0f;
};