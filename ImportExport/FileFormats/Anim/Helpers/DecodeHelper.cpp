#include <Helpers\ByteStream.h>
#include <Quantization\QuantTools.h>
#include "..\Types\Common\QuantMetaTables.h"
#include "DecodeHelper.h"

namespace anim_file
{
	// TODO: REMOVE THIS ONCE WORKS
	size_t DEBUG_translations = 0;
	size_t DEBUG_rotations = 0;

	sm::Vector3 TranslationTrackDecoder::DecodeConstTrack(ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta)
	{
		auto& transMetaId = meta.translationEncodeIds[boneIndex];

		switch (transMetaId.type)
		{
			case TranslationEncodeTypeEnum::Const_Byte3_Trans:
			{
				auto translationSNORM = quant_tools::GetSNORMFloat3FromSINT84(bytes.GetChunk<int8_t>(3).data());
				auto& transRange = meta.ranges.translationRanges[boneIndex];
				return CorrectTranslationRange(translationSNORM, transRange);
			}
			break;

			case TranslationEncodeTypeEnum::Const_Float3_Trans:
			{
				return  bytes.TReadElement<DirectX::XMFLOAT3>();
			}
			break;
		};

		throw std::exception(("QuaternionTrackDecoder::DecodeConstTrack: Unknown compression id, numerical value: " + std::to_string(static_cast<int8_t>(transMetaId.type))).c_str());

		return sm::Vector3::Zero; // TODO: throw exception here, when stuff works
	}

	sm::Vector3 TranslationTrackDecoder::DecodeDynamicTrack(ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta)
	{
		auto& encodeId = meta.translationEncodeIds[boneIndex];

		switch (encodeId.type)
		{
			case TranslationEncodeTypeEnum::Const_Byte3_Trans:
			case TranslationEncodeTypeEnum::Const_Float3_Trans:
			{
				if (m_constTrackIndex >= meta.constTrackFrame.translations.size())
					throw std::exception("TranslationTrackDecoder::DecodeDynamicTrack(): ERROR: Const track index out of bounds!");

				return meta.constTrackFrame.translations[m_constTrackIndex++];
			}
			break;

			case TranslationEncodeTypeEnum::BindPose_Trans:
			{
				if (meta.poSkeletonBindPoseFrame == nullptr) // TODO: maybe not use a pointer, just have param = a reference to a copied object?
					throw std::exception("TranslationTrackDecoder::DecodeDynamicTrack(): ERROR: Bindpose Expected: BindPose==nullptr!");

				if (boneIndex >= meta.poSkeletonBindPoseFrame->translations.size())
					throw std::exception("TranslationTrackDecoder::DecodeDynamicTrack(): ERROR: Bindpose track index out of bounds!");

				return meta.poSkeletonBindPoseFrame->translations[boneIndex];
			}
			break;

			case TranslationEncodeTypeEnum::Data_Byte3_Trans:
			{
				auto translationSNORM = quant_tools::GetSNORMFloat3FromSINT84(bytes.GetChunk<int8_t>(3).data());

				// TODO: REMOVE debugging code
				DEBUG_translations++;
				auto& transRange = meta.ranges.translationRanges[boneIndex];
				return CorrectTranslationRange(translationSNORM, transRange);
			}
			break;

			case TranslationEncodeTypeEnum::Data_Float3_Trans:
			{
				sm::Vector3 translation;
				bytes.Read(&translation, sizeof(sm::Vector3));

				return translation;
			}
			break;
		}

		throw std::exception(("TranslationTrackDecoder::DecodeConstTrack(): Unknown compression id, numerical value: " + std::to_string(static_cast<int8_t>(encodeId.type))).c_str());
	}

	sm::Quaternion QuaternionTrackDecoder::DecodeConstTrack(ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta)
	{
		auto& rotationMetaId = meta.rotationEncodeIds[boneIndex];

		switch (rotationMetaId.type)
		{
			case RotationEncodeTypeEnum::Const_Byte4_Quat:
			{
				auto quaternionSNorm = quant_tools::GetSNormFloat4FromSignedByte4(bytes.GetChunk<int8_t>(4).data());
				auto& quatRange = meta.ranges.quaternionRanges[boneIndex];
				auto decodedQuaternion = CorrectQuaterionRange(quaternionSNorm, quatRange);
				decodedQuaternion.Normalize();

				return decodedQuaternion;
			}
			break;

			case RotationEncodeTypeEnum::Const_Word4_Quat:
			{
				auto quaternionSNorm = quant_tools::GetSNormFloat4FromSignedWord4(bytes.GetChunk<int16_t>(4).data());
				auto& quatRange = meta.ranges.quaternionRanges[boneIndex];
				auto decodedQuaternion = CorrectQuaterionRange(quaternionSNorm, quatRange);
				decodedQuaternion.Normalize();

				return decodedQuaternion;
			}
			break;

			case RotationEncodeTypeEnum::Const_Float4_Quat:
			{
				sm::Quaternion decodedQuaternion = bytes.TReadElement<DirectX::XMFLOAT4>();
				decodedQuaternion.Normalize();

				return decodedQuaternion;
			}
			break;
		}

		throw std::exception((" QuaternionTrackDecoder::DecodeConstTrack: Unknown compression id, numerical value: " + std::to_string(static_cast<int8_t>(rotationMetaId.type))).c_str());

		return sm::Quaternion::Identity; // TODO: throw exception here, when stuff works
	}

	sm::Quaternion QuaternionTrackDecoder::DecodeDynamicTrack(ByteStream& bytes, const uint32_t boneIndex, const CompressionMetaData_V8& meta)
	{
		// TODO: add exceptions to check ranges?		
		auto& rotationMetaId = meta.rotationEncodeIds[boneIndex];

		switch (rotationMetaId.type)
		{
			case RotationEncodeTypeEnum::Const_Byte4_Quat:
			case RotationEncodeTypeEnum::Const_Word4_Quat:
			case RotationEncodeTypeEnum::Const_Float4_Quat:
			{
				auto& constRotation = meta.constTrackFrame.rotations[m_constTrackIndex];
				m_constTrackIndex++;

				return constRotation;
			}
			break;

			case RotationEncodeTypeEnum::Quat_BindPose:
			{
				if (meta.poSkeletonBindPoseFrame == nullptr)  // TODO: maybe not use a pointer, just have param = a reference to a copied object?
					throw std::exception("QuaternionTrackDecoder::DecodeDynamicTrack(): ERROR: Bindpose Expected: BindPose==nullptr!");

				if (boneIndex >= meta.poSkeletonBindPoseFrame->rotations.size())
					throw std::exception("QuaternionTrackDecoder::DecodeDynamicTrack(): ERROR: Bindpose track index out of bounds!");

				return meta.poSkeletonBindPoseFrame->rotations[boneIndex];
			}
			break;

			case RotationEncodeTypeEnum::Data_Byte4_Quat:
			{
				auto quaternionSNorm = quant_tools::GetSNormFloat4FromSignedByte4(bytes.GetChunk<int8_t>(4).data());
				auto& quatRange = meta.ranges.quaternionRanges[boneIndex];
				auto decodedQuaternion = CorrectQuaterionRange(quaternionSNorm, quatRange);
				decodedQuaternion.Normalize();

				return decodedQuaternion;
			}
			break;

			case RotationEncodeTypeEnum::Data_Short4_Quat:
			{
				sm::Quaternion decodedQuaternion = quant_tools::GetSNormFloat4FromSignedWord4(bytes.GetChunk<int16_t>(4).data());
				//auto quaternionSNorm = quant_tools::GetSNormFloat4FromSignedWord4(bytes.GetChunk<int16_t>(4).data());
				//auto& quatRange = meta.ranges.quaternionRanges[boneIndex];
				//auto decodedQuaternion = CorrectQuaterionRange(quaternionSNorm, quatRange);
				decodedQuaternion.Normalize();

				return decodedQuaternion;
			}
			break;

			case RotationEncodeTypeEnum::Data_Float4_Quat:
			{
				sm::Quaternion decodedQuaternion = bytes.TReadElement<DirectX::XMFLOAT4>();
				decodedQuaternion.Normalize();

				return decodedQuaternion;
			}
			break;
		}

		throw std::exception(("QuaternionTrackDecoder(): Unknown compression id, numerical value: " + std::to_string(static_cast<int8_t>(rotationMetaId.type))).c_str());

		return sm::Quaternion::Identity; // TODO: Remove? Ideally this should never be reached, but maybe compiler will complaing about "not all paths return value..."
	}

	sm::Vector3 TranslationTrackDecoder::CorrectTranslationRange(sm::Vector3 inputSNormTranslation, const TranslationRangeElement& range)
	{
		return inputSNormTranslation * range.factor + range.base;
	};

	sm::Quaternion QuaternionTrackDecoder::CorrectQuaterionRange(sm::Vector4 inputSnormQuanterion, const QuanterionRangeElement& range)
	{
		return inputSnormQuanterion * range.factor + range.base;
	};

} // namespace anim_file