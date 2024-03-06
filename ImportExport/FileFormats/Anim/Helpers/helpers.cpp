#include "helpers.h"

#include "..\DataTypes\FileHeader.h"

uint32_t anim_file::GetTWAnimFileVersion(ByteStream& bytes)
{
	anim_file::AnimHeaderCommon in;
	in.dwVersion = bytes.TReadElement<uint32_t>();

	bytes.SetOffset(0); // reset file pointer
	return in.dwVersion;
}
