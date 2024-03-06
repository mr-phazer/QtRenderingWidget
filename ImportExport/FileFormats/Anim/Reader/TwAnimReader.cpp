#include <SimpleMath.h>
#include "..\..\..\..\Rldx\Rldx\Logging\Logging.h"
#include "..\..\RigidModel\DataTypes\MeshEnumsConstants.h"
#include "..\Creators\AnimFileHeaderCreator.h"
#include "..\Creators\BoneTableCreator.h"
#include "..\DataTypes\QuantMetaTable.h"
#include "..\Helpers\helpers.h"
#include "TwAnimReader.h"

using namespace anim_file;

TwAnimFile anim_file::TwAnimFileReader::Read()
{
	ReadHeader();
	ReadBoneTable();

	return m_animFile;
}

void TwAnimFileReader::ReadHeader()
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

void TwAnimFileReader::ReadBoneTable()
{
	BoneTableCreator boneTableCreator;
	m_animFile.boneTable = boneTableCreator.Create(bytes, m_animFile.fileHeader.dwBoneCount);
}

void anim_file::TwAnimFileReader::ReadFrames()
{
	switch (m_animFile.fileHeader.dwVersion)
	{
	case AnimVersionEnum::ANIM_VERSION_7:
		ReadFrames_v7();
		break;

	default:
		throw std::exception(FULL_FUNC_INFO("Unsuported ANIM version.").c_str());
	}

}

void anim_file::TwAnimFileReader::ReadFrames_v7()
{
	auto trackMetaData = ReadTracksMetaTable_v7(m_animFile.fileHeader.dwBoneCount);

	auto constTrackTranslations = bytes.TReadElement<uint32_t>();
	auto constTrackRotations = bytes.TReadElement<uint32_t>();



}


BoneTrackMetaData_V7 anim_file::TwAnimFileReader::ReadTracksMetaTable_v7(uint32_t boneCount)
{

	BoneTrackMetaData_V7 out(boneCount);
	for (auto& itTrackInfo : out.translationTrackInfo)
	{
		itTrackInfo = bytes.TReadElement<int32_t>();
	}

	for (auto& itTrackInfo : out.rotationTrackInfo)
	{
		itTrackInfo = bytes.TReadElement<int32_t>();
	}

	return out;
}

BoneTrackMetaData_V8 anim_file::TwAnimFileReader::ReadTracksMetaTable_v8(uint32_t boneCount)
{
	throw std::exception("NotImplemented");
	return BoneTrackMetaData_V8();
}

