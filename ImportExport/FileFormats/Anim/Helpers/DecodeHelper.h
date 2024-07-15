#pragma once

#include <SimpleMath.h>
#include "Utils\ByteStream.h"

namespace anim_file
{
	struct CompressionMetaData_V8;
	struct TranslationRangeElement;
	struct QuanterionRangeElement;

	class TranslationTrackDecoder
	{
		size_t m_constTrackIndex = 0;
	public:
		sm::Vector3 DecodeConstTrack(utils::ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta);
		sm::Vector3 DecodeDynamicTrack(utils::ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta);

	private:
		static sm::Vector3 CorrectTranslationRange(sm::Vector3 inputSNormTranslation, const TranslationRangeElement& range);
	};

	class QuaternionTrackDecoder
	{
		size_t m_constTrackIndex = 0;
	public:
		sm::Quaternion DecodeConstTrack(utils::ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta);
		sm::Quaternion DecodeDynamicTrack(utils::ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta);

	private:
		static sm::Quaternion CorrectQuaterionRange(sm::Vector4 inputSNormTranslation, const QuanterionRangeElement& range);
	};
}
