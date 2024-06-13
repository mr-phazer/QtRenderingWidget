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

