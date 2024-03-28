// TODO: DirecInput not used (or is it?), find out, if is included by you or an MS header, maybe it can be avoided
#define DIRECTINPUT_HEADER_VERSION  0x0800 

#include "TwAnimReader.h"

#include <Quantization\QuantTools.h>
#include <SimpleMath.h>
#include "..\..\..\..\Rldx\Rldx\Logging\Logging.h"
#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\RigidModel\Types\Common\MeshEnumsConstants.h"
#include "..\Creators\AnimFileHeaderCreator.h"
#include "..\Creators\BoneTableCreator.h"
#include "..\Helpers\DecodeHelper.h"
#include "..\Types\Common\AnimFrameCommon.h"
#include "..\Types\Common\FrameHeaders.h"
#include "..\Types\Common\QuantMetaTables.h"
#include "..\Types\Common\TwAnimFile.h"

namespace anim_file
{

	AnimFile TwAnimFileReader::Read(ByteStream& inBytes)
	{
		ReadFileHeader(inBytes);
		ReadBoneTable(inBytes);

		SubClipHeaderV8 subClipHeader;
		subClipHeader.subClipCounts = 1; // pre-v8 ANIM only has 1 clip per file
		if (m_animFile.fileHeader.dwVersion == Rmv2VersionEnum::RMV2_V8)
		{
			subClipHeader = ReadSubsClipHeader_v8(inBytes);
		}

		// TODO: add "for subclipsCount" loop around this, so it matches ANIM v8
		for (size_t i = 0; i < subClipHeader.subClipCounts; i++)
		{
			ReadSubClip(inBytes, subClipHeader);
		}

		return m_animFile;
	}

	uint32_t TwAnimFileReader::GetTWAnimFileVersion(ByteStream& bytes)
	{
		AnimHeaderCommon in;
		in.dwVersion = bytes.TReadElement<uint32_t>();

		bytes.SetOffset(0); // reset file pointer
		return in.dwVersion;
	}

	void anim_file::TwAnimFileReader::ReadFileHeader(ByteStream& inBytes)
	{
		auto version = GetTWAnimFileVersion(inBytes);

		switch (version)
		{
			case ANIM_VERSION_5:
				m_animFile.fileHeader = Anim_V6_HeaderFileCommonCreator().Create(inBytes);
				break;

			case ANIM_VERSION_7:
				m_animFile.fileHeader = Anim_V7_HeaderFileCommonCreator().Create(inBytes);
				break;

			case ANIM_VERSION_8:
				m_animFile.fileHeader = Anim_V8_HeaderFileCommonCreator().Create(inBytes);
				break;

			default:
				throw std::exception(("TwAnimFileReader::ReadFileHeader(): ERROR: Not a supported TW ANIM file, unknown version numeric: ", to_string(version)).c_str());
		}
	}

	void TwAnimFileReader::ReadBoneTable(ByteStream& inBytes)
	{
		BoneTableCreator boneTableCreator;
		m_animFile.boneTable = boneTableCreator.Create(inBytes, m_animFile.fileHeader.dwBoneCount);
	}

	SubClipHeaderV8 TwAnimFileReader::ReadSubsClipHeader_v8(ByteStream& inBytes)
	{
		SubClipHeaderV8 out;
		inBytes.Read(&out.dwUnknown1, 4);
		inBytes.Read(&out.subClipCounts, 4);

		return out;
	}

	void TwAnimFileReader::ReadSubClip(ByteStream& inBytes, const SubClipHeaderV8& subclipHeader)
	{
		switch (m_animFile.fileHeader.dwVersion)
		{
			case AnimVersionEnum::ANIM_VERSION_5:
			case AnimVersionEnum::ANIM_VERSION_7:
				ReadClip_v7(inBytes);
				break;

			case AnimVersionEnum::ANIM_VERSION_8:
				ReadClip_v8(inBytes);
				break;

			default:
				throw std::exception(FULL_FUNC_INFO("Unsuported ANIM version.").c_str());
		};

	};

	void TwAnimFileReader::ReadClip_v5_v7(ByteStream& inBytes)
	{
		auto tracksMetaData = ReadCompressionMetaData_v5_v7(inBytes, m_animFile.fileHeader.dwBoneCount);

		auto subClipHeader = ReadSubClipFramesHeader(inBytes);

		m_animFile.frames.resize(subClipHeader.frameCount);
		for (auto& itFrame : m_animFile.frames)
		{
			itFrame = DecodeFrame_V7(inBytes, tracksMetaData, subClipHeader, nullptr, (!m_bindPose.frames.empty()) ? &m_bindPose.frames[0] : nullptr);
		};
	};

	void TwAnimFileReader::ReadClip_v7(ByteStream& inBytes)
	{
		auto tracksMetaData = ReadCompressionMetaData_v5_v7(inBytes, m_animFile.fileHeader.dwBoneCount);

		auto constTrackFrameHeader = ReadConstTracksFrameHeader_v7(inBytes);
		auto constTrackFrame = ReadConstTracksFrame_v7(inBytes, constTrackFrameHeader);

		auto subClipHeader = ReadSubClipFramesHeader(inBytes);

		m_animFile.frames.resize(subClipHeader.frameCount);
		for (auto& itFrame : m_animFile.frames)
		{
			itFrame = DecodeFrame_V7(inBytes, tracksMetaData, subClipHeader, &constTrackFrame, (!m_bindPose.frames.empty()) ? &m_bindPose.frames[0] : nullptr);
		};
	};


	void TwAnimFileReader::ReadClip_v8(ByteStream& inBytes)
	{
		auto compressionMetaData = ReadCompressionMetaData_v8(inBytes, m_animFile.fileHeader.dwBoneCount);
		auto constTracksFrameHeader = ReadConstTracksFrameHeader_v7(inBytes);
		compressionMetaData.constTrackFrame = ReadConstTracksFrame_V8(inBytes, compressionMetaData, constTracksFrameHeader); // store const tracks frame, for decoding frames

		auto subClipHeader = ReadSubClipFramesHeader(inBytes);
		for (size_t i = 0; i < subClipHeader.frameCount; i++)
		{
			auto decodedFrame = DecodeFrame_V8(inBytes, compressionMetaData);
			m_animFile.frames.push_back(decodedFrame);
		}

		// TODO: REMOVE DEBUGGING CODE
		auto DEBUG_BREAK_1 = 1;
	};

	AnimFrameCommon TwAnimFileReader::ReadConstTracksFrame_v7(ByteStream& inBytes, const FrameHeaderCommon& constTrackFrameHeader)
	{
		AnimFrameCommon outFrame(constTrackFrameHeader.translationCount, constTrackFrameHeader.rotationCount);

		for (auto& itTrans : outFrame.translations) {
			itTrans = inBytes.TReadElement<sm::Vector3>();
		}

		for (auto& itRot : outFrame.rotations) {
			itRot = quant_tools::GetSNormFloat4FromSignedWord4(inBytes.TReadElement<DirectX::PackedVector::XMSHORT4>());
		}

		return outFrame;
	};

	AnimFrameCommon TwAnimFileReader::DecodeFrame_V7(
		ByteStream& inBytes,
		const CompressionMetaData_V5_V7& metaTable,
		const FrameHeaderCommon& frameHeader,
		const AnimFrameCommon* pConstTrackFrame,
		const AnimFrameCommon* pBindPoseFrame)
	{
		const auto constTrackErrorException = std::exception("DecodeFrame_V7(): Error: Const Track Expected");
		const auto bindPoseErrorException = std::exception("DecodeFrame_V7(): Error: BindPose Expected");

		AnimFrameCommon outFrame(metaTable.boneCount, metaTable.boneCount);

		for (size_t iBone = 0; iBone < metaTable.boneCount; iBone++)
		{
			auto& transMeta = metaTable.translationInfo[iBone];

			switch (transMeta.GetTrackSourceState())
			{
				case AnimTrackSourceEnum::FrameData:
				{
					outFrame.translations[iBone] = inBytes.TReadElement<sm::Vector3>();
				}
				break;

				case AnimTrackSourceEnum::ConstTrack:
				{
					if (pConstTrackFrame == nullptr) throw constTrackErrorException;

					outFrame.translations[iBone] = pConstTrackFrame->translations[transMeta.GetConstTrackIndex()];
				}
				break;

				case AnimTrackSourceEnum::BindPose:
				{
					if (pBindPoseFrame == nullptr) throw bindPoseErrorException;

					outFrame.translations[iBone] = pBindPoseFrame->translations[iBone];
				}
				break;
			};
		};

		for (size_t iBone = 0; iBone < metaTable.boneCount; iBone++)
		{
			switch (metaTable.rotationTrackInfo[iBone].GetTrackSourceState())
			{
				case AnimTrackSourceEnum::FrameData:
				{
					outFrame.rotations[iBone] = quant_tools::GetSNormFloat4FromSignedWord4(inBytes.TReadElement<DirectX::PackedVector::XMSHORT4>());
				}
				break;

				case AnimTrackSourceEnum::ConstTrack:
				{
					if (pConstTrackFrame == nullptr) throw constTrackErrorException;

					outFrame.rotations[iBone] = pConstTrackFrame->rotations[metaTable.rotationTrackInfo[iBone].GetConstTrackIndex()];
				}
				break;

				case AnimTrackSourceEnum::BindPose:
				{
					if (pBindPoseFrame == nullptr) throw bindPoseErrorException;

					outFrame.rotations[iBone] = pBindPoseFrame->rotations[iBone];
				}
				break;
			};
		};

		return outFrame;
	}

	AnimFrameCommon TwAnimFileReader::ReadConstTracksFrame_V8(ByteStream& inBytes, const CompressionMetaData_V8& metaData, const FrameHeaderCommon& frameHeader) const
	{
		AnimFrameCommon constOutFrame;
		constOutFrame.translations.reserve(frameHeader.translationCount);
		constOutFrame.rotations.reserve(frameHeader.rotationCount);

		TranslationTrackDecoder translationTrackDecoder;
		for (uint32_t iBone = 0; iBone < metaData.translationEncodeIds.size(); iBone++)
		{
			if (!metaData.translationEncodeIds[iBone].IsConstTrackData()) { continue; } // we are only reading track consts frame data

			auto newTranslation = translationTrackDecoder.DecodeConstTrack(inBytes, iBone, metaData);
			constOutFrame.translations.push_back(newTranslation);
		}

		QuaternionTrackDecoder quatTrackDecoder;
		for (uint32_t iBone = 0; iBone < metaData.translationEncodeIds.size(); iBone++)
		{
			if (!metaData.rotationEncodeIds[iBone].IsConstTrackData()) { continue; } // we are only reading track consts frame data

			auto newQuat = quatTrackDecoder.DecodeConstTrack(inBytes, iBone, metaData);
			constOutFrame.rotations.push_back(newQuat);
		}

		return constOutFrame;
	};

	AnimFrameCommon TwAnimFileReader::DecodeFrame_V8(ByteStream& inBytes, const CompressionMetaData_V8& metaData) const
	{
		AnimFrameCommon newFrame;

		TranslationTrackDecoder translationTrackDecoder;
		for (uint32_t iBone = 0; iBone < m_animFile.fileHeader.dwBoneCount; iBone++)
		{
			auto newTranslation = translationTrackDecoder.DecodeDynamicTrack(inBytes, iBone, metaData);

			// TODO: REMOVE DEBUGGING CODE
			logging::LogActionSuccess("Decoded translation for bone: " + to_string(iBone) + " : " +
									  "(" + to_string(newTranslation.x) + ", " + to_string(newTranslation.y) + ", " + to_string(newTranslation.z) + ")");

			newFrame.translations.push_back(newTranslation);
		}

		QuaternionTrackDecoder quatTrackDecoder;
		for (uint32_t iBone = 0; iBone < m_animFile.fileHeader.dwBoneCount; iBone++)
		{
			auto newQuat = quatTrackDecoder.DecodeDynamicTrack(inBytes, iBone, metaData);

			logging::LogActionSuccess("Decoded Quaternion for bone: " + to_string(iBone) + " : " +
									  "(" + to_string(newQuat.x) + ", " + to_string(newQuat.y) + ", " + to_string(newQuat.z) + ", " + to_string(newQuat.w) + ")");

			logging::LogActionSuccess("Quaternion length: " + to_string(newQuat.Length()));

			newFrame.rotations.push_back(newQuat);
		}

		return newFrame;
	};

	FrameHeaderCommon TwAnimFileReader::ReadConstTracksFrameHeader_v7(ByteStream& inBytes)
	{
		FrameHeaderCommon out;
		out.translationCount = inBytes.TReadElement<uint32_t>();
		out.rotationCount = inBytes.TReadElement<uint32_t>();
		out.frameCount = 1;

		return out;
	}

	FrameHeaderCommon TwAnimFileReader::ReadSubClipFramesHeader(ByteStream& inBytes)
	{
		FrameHeaderCommon out;
		out.translationCount = inBytes.TReadElement<uint32_t>();
		out.rotationCount = inBytes.TReadElement<uint32_t>();
		out.frameCount = inBytes.TReadElement<uint32_t>();

		return out;
	}

	CompressionMetaData_V5_V7 TwAnimFileReader::ReadCompressionMetaData_v5_v7(ByteStream& inBytes, uint32_t boneCount)
	{
		CompressionMetaData_V5_V7 out(boneCount);

		for (auto& itTrackInfo : out.translationInfo)
		{
			inBytes.Read(&itTrackInfo, sizeof(int32_t));
		}

		for (auto& itTrackInfo : out.rotationTrackInfo)
		{
			inBytes.Read(&itTrackInfo, sizeof(int32_t));
		}

		return out;
	};

	CompressionMetaData_V8 TwAnimFileReader::ReadCompressionMetaData_v8(ByteStream& inBytes, uint32_t boneCount)
	{
		CompressionMetaData_V8 outMetaTable;

		outMetaTable.translationEncodeIds.resize(boneCount);
		outMetaTable.rotationEncodeIds.resize(boneCount);

		inBytes.Read(outMetaTable.translationEncodeIds.data(), boneCount * sizeof(int8_t));
		inBytes.Read(outMetaTable.rotationEncodeIds.data(), boneCount * sizeof(int8_t));

		outMetaTable.ranges.translationRangeTableLength = inBytes.TReadElement<uint32_t>();
		outMetaTable.ranges.quaterionRangeTableLength = inBytes.TReadElement<uint32_t>();

		outMetaTable.ranges.translationRanges.resize(outMetaTable.ranges.translationRangeTableLength);
		outMetaTable.ranges.quaternionRanges.resize(outMetaTable.ranges.quaterionRangeTableLength);

		inBytes.Read(outMetaTable.ranges.translationRanges.data(), sizeof(TranslationRangeElement) * outMetaTable.ranges.translationRangeTableLength);
		inBytes.Read(outMetaTable.ranges.quaternionRanges.data(), sizeof(QuanterionRangeElement) * outMetaTable.ranges.quaterionRangeTableLength);

		outMetaTable.poSkeletonBindPoseFrame = m_bindPose.frames.empty() ? nullptr : &m_bindPose.frames[0];

		return outMetaTable;
	};
}