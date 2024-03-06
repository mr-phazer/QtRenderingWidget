#pragma once

#include "..\..\..\Helpers\ByteStream.h"
#include "..\DataTypes\QuantMetaTable.h"
#include "..\DataTypes\TwAnimFile.h"

namespace anim_file
{
	class TwAnimFileReader
	{
		TwAnimFile m_animFile;
		ByteStream& bytes;

	public:
		explicit TwAnimFileReader(ByteStream& bytes) : bytes(bytes) {};

		TwAnimFile Read();

	private:
		void ReadHeader();
		void ReadBoneTable();
		void ReadFrames();
		void ReadFrames_v7();
		void ReadFrames_v8();



		BoneTrackMetaData_V7 ReadTracksMetaTable_v7(uint32_t boneCount);
		BoneTrackMetaData_V8 ReadTracksMetaTable_v8(uint32_t boneCount);
	};
}