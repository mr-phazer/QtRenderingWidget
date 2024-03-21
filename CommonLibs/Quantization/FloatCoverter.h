#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <SimpleMath.h>
#include <type_traits>

struct FloatConverter {

	template <typename UINT_TYPE>
	static inline constexpr float GetUNormFloatFromUINT(UINT_TYPE integerfixedPoint)
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
	static inline constexpr float TGetSNormFloatFromInt(UINT_TYPE integerfixedPoint)
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
	inline static constexpr DirectX::XMFLOAT3 TGetSNORMFloat3(const CONST_BUF_DATA_TYPE& vIn)
	{
		return
		{
			(static_cast<float>(vIn.x) / 127.0f) - 1.f,
			(static_cast<float>(vIn.y) / 127.0f) - 1.f,
			(static_cast<float>(vIn.z) / 127.0f) - 1.f
		};
	};


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

	inline static constexpr float GetSNormFloatFromInt16(int16_t fixedPoint)
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

	inline static DirectX::XMFLOAT3 GetSNORMFloat3FromSINT84(DirectX::PackedVector::XMUBYTE4 vIn)
	{
		return
		{
			GetSNormFloatFromSINT8(vIn.x),
			GetSNormFloatFromSINT8(vIn.y),
			GetSNormFloatFromSINT8(vIn.z),
		};
	};

	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedInt4(DirectX::PackedVector::XMBYTE4 vIn)
	{
		return
		{
			GetSNormFloatFromByte(vIn.x),
			GetSNormFloatFromByte(vIn.y),
			GetSNormFloatFromByte(vIn.z),
			GetSNormFloatFromByte(vIn.w)
		};
	};

	inline static DirectX::XMFLOAT4 GetSNormFloat4FromSignedInt4(DirectX::PackedVector::XMSHORT4 vIn)
	{
		return	{
			GetSNormFloatFromInt16(vIn.x),
			GetSNormFloatFromInt16(vIn.y),
			GetSNormFloatFromInt16(vIn.z),
			GetSNormFloatFromInt16(vIn.w),
		};
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

	template <typename CONST_BUF_DATA_TYPE>
	inline static DirectX::XMFLOAT4 GetFloat4FromHalf4(const CONST_BUF_DATA_TYPE& in)
	{
		return {
			DirectX::PackedVector::XMConvertHalfToFloat(in.x),
			DirectX::PackedVector::XMConvertHalfToFloat(in.y),
			DirectX::PackedVector::XMConvertHalfToFloat(in.z),
			DirectX::PackedVector::XMConvertHalfToFloat(in.w)
		};
	}


	template <typename CONST_BUF_DATA_TYPE>
	inline static DirectX::XMFLOAT2 GetFloat2FromHalf2(const CONST_BUF_DATA_TYPE& in)
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