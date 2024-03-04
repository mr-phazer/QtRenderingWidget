
#include "AnimFIleHeaderCreator.h"

anim_file::AnimHeaderCommon Anim_V6_HeaderFileCommonCreator::Create(ByteStream& bytes)
{
	anim_file::AnimHeaderCommon in;
	in.dwVersion = bytes.TReadElement<uint32_t>();
	in.dwUnknown = bytes.TReadElement<uint32_t>();
	in.fFrameRate = bytes.TReadElement<float>();
	in.skeletonName = bytes.ReadLengthPrefixed16StringA();
	in.fEndTime = bytes.TReadElement<float>();
	in.dwBoneCount = bytes.TReadElement<uint32_t>();

	return in;
}

anim_file::AnimHeaderCommon Anim_V7_HeaderFileCommonCreator::Create(ByteStream& bytes)
{
	anim_file::AnimHeaderCommon in;
	in.dwVersion = bytes.TReadElement<uint32_t>();
	in.dwUnknown = bytes.TReadElement<uint32_t>();
	in.fFrameRate = bytes.TReadElement<float>();
	in.skeletonName = bytes.ReadLengthPrefixed16StringA();

	in.subHeader_V7.flagCount = bytes.TReadElement<uint32_t>();

	if (in.subHeader_V7.flagCount != 00) // is there sub header?
	{
		in.subHeader_V7.strText = bytes.TReadElement<uint32_t>();
	}

	in.fEndTime = bytes.TReadElement<float>();
	in.dwBoneCount = bytes.TReadElement<uint32_t>();


	return in;
}


