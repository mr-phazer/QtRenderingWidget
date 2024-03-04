#include "..\DataTypes\AnimHeaderCommon.h"
#include "AnimFIleHeaderCreator.h"

anim::AnimHeaderCommon Anim_V6_HeaderFileCommon::Create(ByteStream& bytes)
{
	anim::AnimHeaderCommon in;
	in.dwVersion = bytes.TReadElement<uint32_t>();
	in.dwUnknown = bytes.TReadElement<uint32_t>();
	in.fFrameRate = bytes.TReadElement<float>();
	in.skeletonName = bytes.ReadLengthPrefixedStringA<uint16_t>();
}
