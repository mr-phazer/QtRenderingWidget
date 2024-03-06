#pragma once

#include <cstdint>
#include <SimpleMath.h>
#include <vector>

namespace anim_file
{
	struct TranslationRange
	{
		sm::Vector3 min = { 0,0,0 };
		sm::Vector3 max = { 0,0,0 };
	};

	struct QuanterionRange
	{
		sm::Quaternion min = { 0,0,0,0 };
		sm::Quaternion max = { 0,0,0,0 };
	};


	struct BoneTrackMetaData_V8
	{
		uint32_t boneCount;

		std::vector<int8_t> translationTrackInfo;
		std::vector<int8_t> rotationTrackInfo;

		std::vector<TranslationRange> translationRange;
		std::vector<QuanterionRange> quaternionRange;
	};

	struct BoneTrackMetaData_V7
	{
		BoneTrackMetaData_V7(uint32_t boneCountValue)
			:
			boneCount(boneCountValue),
			rotationTrackInfo(boneCount, { 0 }),
			translationTrackInfo(boneCount, { 0 })
		{}

		static constexpr uint32_t ConstTrackFlagMask = 10000;

		int32_t boneCount;
		std::vector<int32_t> translationTrackInfo;
		std::vector<int32_t> rotationTrackInfo;
	};
}