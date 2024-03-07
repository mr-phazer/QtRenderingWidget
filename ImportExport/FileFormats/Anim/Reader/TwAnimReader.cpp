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

TwAnimFile anim_file::TwAnimFileReader::Read(ByteStream& bytes)
{
	ReadHeader(bytes);
	ReadBoneTable(bytes);
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
			throw std::exception("NOT IMPLEMENT FOR OTHER ANIM File Versions");
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
			ReadFrames_v7(bytes);
			break;

		default:
			throw std::exception(FULL_FUNC_INFO("Unsuported ANIM version.").c_str());
	}

}

void anim_file::TwAnimFileReader::ReadFrames_v7(ByteStream& bytes)
{
	auto tracksMetaData = ReadTracksMetaTable_v7(bytes, m_animFile.fileHeader.dwBoneCount);

	auto constTrackFrameHeader = ReadConstTrackFrameHeader(bytes);
	auto constTrackFrame = ReadConstTracksFrame_v7(bytes, constTrackFrameHeader);

	auto subClipHeader = ReadPartHeader(bytes);

	m_animFile.frames.resize(subClipHeader.frameCount);
	for (auto& itFrame : m_animFile.frames)
	{
		itFrame = ReadSingleFrame_v7(bytes, tracksMetaData, subClipHeader, nullptr, nullptr);
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

AnimFrameCommon anim_file::TwAnimFileReader::ReadSingleFrame_v7(
	ByteStream& bytes,
	const BoneTrackMetaData_V7& metaTable,
	const FrameHeaderCommon& frameHeader,
	const AnimFrameCommon* constTrackFrame,
	const AnimFrameCommon* bindPoseFrame)
{
	AnimFrameCommon outFrame(frameHeader.translationCount, frameHeader.rotationCount);

	size_t bindPoseTransIndex = 0;
	for (size_t i = 0; i < frameHeader.translationCount; i++)
	{
		auto& transMeta = metaTable.translationInfo[i];

		switch (transMeta.GetTrackSourceState())
		{
			case AnimTrackSourceEnum::FrameData:
				outFrame.translations[i] = bytes.TReadElement<sm::Vector3>();
				break;

			case AnimTrackSourceEnum::ConstTrack:
				if (bindPoseFrame == nullptr) throw("ReadSingleFrame_v7(): Error, const track expected");
				outFrame.translations[i] = constTrackFrame->translations[transMeta.GetConstTrackIndex()];
				break;

			case AnimTrackSourceEnum::BindPose:
				if (bindPoseFrame == nullptr) throw("ReadSingleFrame_v7(): Error, bindpose expected");

				outFrame.translations[i] = bindPoseFrame->translations[bindPoseTransIndex++];
				break;
		}
	};

	size_t bindPoseRotationIndex = 0;
	for (size_t i = 0; i < frameHeader.rotationCount; i++)
	{
		switch (metaTable.rotationTrackInfo[i].GetTrackSourceState())
		{
			case AnimTrackSourceEnum::FrameData:
				outFrame.rotations[i] = FloatConverter::GetSNormFloat4FromSignedInt4(bytes.TReadElement<DirectX::PackedVector::XMSHORT4>());
				break;

			case AnimTrackSourceEnum::ConstTrack:
				if (constTrackFrame == nullptr)	throw std::exception("ReadSingleFrame_v7(): Error, const track expected");

				outFrame.rotations[i] = constTrackFrame->rotations[metaTable.rotationTrackInfo[i].GetConstTrackIndex()];
				break;

			case AnimTrackSourceEnum::BindPose:
				if (constTrackFrame == nullptr)	throw("ReadSingleFrame_v7(): Error, bindpose expected");

				outFrame.rotations[i] = bindPoseFrame->rotations[bindPoseRotationIndex++];
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

FrameHeaderCommon anim_file::TwAnimFileReader::ReadPartHeader(ByteStream& bytes)
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

