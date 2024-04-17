#pragma once

#include <SimpleMath.h>
#include <vector>
#include "..\..\Types\Common\AnimFrameCommon.h"

namespace anim_file
{
	enum  class TranslationEncodeTypeEnum : int8_t
	{
		BindPose_Trans = 0,

		Data_Byte3_Trans = 3,
		Const_Byte3_Trans = -3,

		Data_Float3_Trans = 12,
		Const_Float3_Trans = -12,
	};

	enum class RotationEncodeTypeEnum : int8_t
	{
		Quat_BindPose = 0,

		Data_Byte4_Quat = 4,
		Const_Byte4_Quat = -4,

		Data_Short4_Quat = 8,
		Const_Word4_Quat = -8,

		Data_Float4_Quat = 16,
		Const_Float4_Quat = -16,
	};

	enum BoneTrackDataSourceEnum_v5_v7
	{
		BindPose, ConstTrack, FrameData
	};

	class MetaElement_v7
	{
		int32_t m_metaValue;

	public:
		BoneTrackDataSourceEnum_v5_v7 GetTrackSourceState() const;

		uint32_t GetConstTrackIndex() const;
	};

	struct TranslationRangeElement
	{
		sm::Vector3 factor = { 0,0,0 };
		sm::Vector3 base = { 0,0,0 };
	};

	struct QuanterionRangeElement
	{
		sm::Vector4 factor = { 0,0,0,0 };
		sm::Vector4 base = { 0,0,0,0 };
	};

	// TODO: maybe use this inte
	struct TranslationMetaDataElement_V8
	{
		TranslationEncodeTypeEnum type;

		constexpr bool IsConstTrackData() const
		{
			return (static_cast<int8_t>(type) < 0);
		}
	};

	struct QuaternionMetaDataElement_V8
	{
		RotationEncodeTypeEnum type;

		constexpr bool IsConstTrackData() const
		{
			return (static_cast<int8_t>(type) < 0);
		}
	};

	struct CompressionMetaData_V8
	{
		std::vector<TranslationMetaDataElement_V8> translationEncodeIds;
		std::vector<QuaternionMetaDataElement_V8> rotationEncodeIds;

		struct DeQuantRangeTables
		{
			uint32_t translationRangeTableLength = 0;
			uint32_t quaterionRangeTableLength = 0;

			std::vector<TranslationRangeElement> translationRanges;
			std::vector<QuanterionRangeElement> quaternionRanges;
		}
		ranges;

		AnimFrameCommon constTrackFrame;
		AnimFrameCommon* poSkeletonBindPoseFrame = nullptr;
	};

	struct CompressionMetaData_V5_V7
	{
		CompressionMetaData_V5_V7(uint32_t boneCountValue)
			:
			boneCount(boneCountValue),
			rotationTrackInfo(boneCount),
			translationInfo(boneCount)
		{}

		static constexpr uint32_t ConstTrackFlagMask = 10000;

		int32_t boneCount;
		std::vector<MetaElement_v7> translationInfo;
		std::vector<MetaElement_v7> rotationTrackInfo;
	};
}