#include <SimpleMath.h>
#include "..\..\..\..\Rldx\Rldx\Logging\Logging.h"
#include "..\..\..\Helpers\ByteStream.h"
#include "..\..\..\Helpers\DataConversion.h"
#include "..\..\RigidModel\Types\Common\MeshEnumsConstants.h"
#include "..\Creators\AnimFileHeaderCreator.h"
#include "..\Creators\BoneTableCreator.h"
#include "..\Helpers\helpers.h"
#include "..\Types\Common\QuantMetaTables.h"
#include "TwAnimReader.h"

using namespace anim_file;

AnimFile anim_file::TwAnimFileReader::Read(ByteStream& bytes, AnimFile* poBindPose)
{
	m_poBindPose = poBindPose;

	ReadHeader(bytes);
	ReadBoneTable(bytes);

	// TODO: add "for subclipsCount" loop around this, so it matches ANIM v8
	ReadSubClip(bytes);

	return m_animFile;
}

void anim_file::TwAnimFileReader::ReadHeader(ByteStream& bytes)
{
	auto version = GetTWAnimFileVersion(bytes);

	switch (version)
	{
		case ANIM_VERSION_6:
			m_animFile.fileHeader = Anim_V6_HeaderFileCommonCreator().Create(bytes);
			break;

		case ANIM_VERSION_7:
			m_animFile.fileHeader = Anim_V7_HeaderFileCommonCreator().Create(bytes);
			break;

		default:
			throw std::exception("ERROR: Not a supported TW ANIM version!");
	}
}

void anim_file::TwAnimFileReader::ReadBoneTable(ByteStream& bytes)
{
	BoneTableCreator boneTableCreator;
	m_animFile.boneTable = boneTableCreator.Create(bytes, m_animFile.fileHeader.dwBoneCount);
}

void anim_file::TwAnimFileReader::ReadSubClip(ByteStream& bytes)
{
	switch (m_animFile.fileHeader.dwVersion)
	{
		case AnimVersionEnum::ANIM_VERSION_7:
			ReadClip_v7(bytes);
			break;

		default:
			throw std::exception(FULL_FUNC_INFO("Unsuported ANIM version.").c_str());
	}

}

void anim_file::TwAnimFileReader::ReadClip_v7(ByteStream& bytes)
{
	auto tracksMetaData = ReadTracksMetaTable_v7(bytes, m_animFile.fileHeader.dwBoneCount);

	auto constTrackFrameHeader = ReadConstTrackFrameHeader(bytes);
	auto constTrackFrame = ReadConstTracksFrame_v7(bytes, constTrackFrameHeader);

	auto subClipHeader = ReadSubClipHeader(bytes);

	m_animFile.frames.resize(subClipHeader.frameCount);
	for (auto& itFrame : m_animFile.frames)
	{
		itFrame = DecodeFrame_V7(bytes, tracksMetaData, subClipHeader, nullptr, nullptr);
	}
}

AnimFrameCommon anim_file::TwAnimFileReader::ReadConstTracksFrame_v7(ByteStream& bytes, const FrameHeaderCommon& constTrackFrameHeader)
{
	AnimFrameCommon outFrame(constTrackFrameHeader.translationCount, constTrackFrameHeader.rotationCount);

	for (auto& itTrans : outFrame.translations) {
		itTrans = bytes.TReadElement<sm::Vector3>();
	}

	for (auto& itRot : outFrame.rotations) {
		itRot = FloatConverter::GetSNormFloat4FromSignedInt4(bytes.TReadElement<DirectX::PackedVector::XMSHORT4>());
	}

	return outFrame;
}

AnimFrameCommon anim_file::TwAnimFileReader::DecodeFrame_V7(
	ByteStream& bytes,
	const BoneTrackMetaData_V7& metaTable,
	const FrameHeaderCommon& frameHeader,
	const AnimFrameCommon* pConstTrackFrame,
	const AnimFrameCommon* pBindPoseFrame)
{
	const auto constTrackErrorMessage = "DecodeFrame_V7(): Error: Const Track Expected";
	const auto bindPoseErroMessage = "DecodeFrame_V7(): Error: BindPose Expected";

	AnimFrameCommon outFrame(frameHeader.translationCount, frameHeader.rotationCount);

	size_t translationFrameDataIndex = 0;
	for (size_t iBone = 0; iBone < frameHeader.translationCount; iBone++)
	{
		auto& transMeta = metaTable.translationInfo[iBone];

		switch (transMeta.GetTrackSourceState())
		{
			case AnimTrackSourceEnum::FrameData:
				outFrame.translations[translationFrameDataIndex] = bytes.TReadElement<sm::Vector3>();
				translationFrameDataIndex++;
				break;

			case AnimTrackSourceEnum::ConstTrack:
				if (pConstTrackFrame == nullptr) throw(constTrackErrorMessage);
				outFrame.translations[iBone] = pConstTrackFrame->translations[transMeta.GetConstTrackIndex()];
				break;

			case AnimTrackSourceEnum::BindPose:
				if (pBindPoseFrame == nullptr) throw(bindPoseErroMessage);

				outFrame.translations[iBone] = pBindPoseFrame->translations[iBone];
				break;
		}
	};

	size_t rotationFrameDataIndex = 0;
	for (size_t iBone = 0; iBone < frameHeader.rotationCount; iBone++)
	{
		switch (metaTable.rotationTrackInfo[iBone].GetTrackSourceState())
		{
			case AnimTrackSourceEnum::FrameData:
				outFrame.rotations[rotationFrameDataIndex] = FloatConverter::GetSNormFloat4FromSignedInt4(bytes.TReadElement<DirectX::PackedVector::XMSHORT4>());
				rotationFrameDataIndex++;
				break;

			case AnimTrackSourceEnum::ConstTrack:
				if (pConstTrackFrame == nullptr) throw std::exception(constTrackErrorMessage);

				outFrame.rotations[iBone] = pConstTrackFrame->rotations[metaTable.rotationTrackInfo[iBone].GetConstTrackIndex()];
				break;

			case AnimTrackSourceEnum::BindPose:
				if (pBindPoseFrame == nullptr) throw(bindPoseErroMessage);

				outFrame.rotations[iBone] = pBindPoseFrame->rotations[iBone];
				break;
		}
	}

	return outFrame;
}

FrameHeaderCommon anim_file::TwAnimFileReader::ReadConstTrackFrameHeader(ByteStream& bytes)
{
	FrameHeaderCommon out;
	out.translationCount = bytes.TReadElement<uint32_t>();
	out.rotationCount = bytes.TReadElement<uint32_t>();
	out.frameCount = 1;

	return out;
}

FrameHeaderCommon anim_file::TwAnimFileReader::ReadSubClipHeader(ByteStream& bytes)
{
	FrameHeaderCommon out;
	out.translationCount = bytes.TReadElement<uint32_t>();
	out.rotationCount = bytes.TReadElement<uint32_t>();
	out.frameCount = bytes.TReadElement<uint32_t>();

	return out;
}


BoneTrackMetaData_V7 anim_file::TwAnimFileReader::ReadTracksMetaTable_v7(ByteStream& bytes, uint32_t boneCount)
{

	BoneTrackMetaData_V7 out(boneCount);
	for (auto& itTrackInfo : out.translationInfo)
	{
		bytes.Read(&itTrackInfo, sizeof(int32_t));
	}

	for (auto& itTrackInfo : out.rotationTrackInfo)
	{
		bytes.Read(&itTrackInfo, sizeof(int32_t));
	}

	return out;
}

BoneTrackMetaData_V8 anim_file::TwAnimFileReader::ReadTracksMetaTable_v8(ByteStream& bytes, uint32_t boneCount)
{
	throw std::exception("NotImplemented");
	return BoneTrackMetaData_V8();
}

