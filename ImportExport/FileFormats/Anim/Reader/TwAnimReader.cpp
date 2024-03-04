#include "TwAnimReader.h"
#include "..\Helpers\helpers.h"
#include "..\Creators\AnimFileHeaderCreator.h"
#include "..\Creators\BoneTableCreator.h"

using namespace anim_file;

TwAnimFile anim_file::TwANIMFileReader::Read(ByteStream& bytes)
{
	ReadHeader(bytes);
	ReadBoneTable(bytes, m_animFile.fileHeader.dwBoneCount);
}

void TwANIMFileReader::ReadHeader(ByteStream& bytes)
{
	auto version = GetTWAnimFileVersion(bytes);

	switch (version)
	{
	case ANIM_VERSION_6:
		m_animFile.fileHeader =	Anim_V6_HeaderFileCommonCreator().Create(bytes);
		break;

	case ANIM_VERSION_7:
		m_animFile.fileHeader =	Anim_V7_HeaderFileCommonCreator().Create(bytes);
		break;

	}

}

void TwANIMFileReader::ReadBoneTable(ByteStream& bytes, size_t boneCount)
{	
	BoneTableCreator(m_animFile.boneTable).Create(bytes, boneCount);
}
