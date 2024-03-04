#pragma once

#include "..\DataTypes\TwAnimFile.h"
#include "..\..\..\Helpers\ByteStream.h"

namespace anim_file
{
	class TwANIMFileReader
	{
		TwAnimFile m_animFile;
					
	public:
		TwAnimFile Read(ByteStream& bytes);

	private:
		void ReadHeader(ByteStream&);
		void ReadBoneTable(ByteStream& bytes, size_t boneCount);
	};
}