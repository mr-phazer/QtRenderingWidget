#include "FileHeader.h"

bool anim_file::AnimHeaderCommon::IsContentValid()
{
	if (
		dwVersion > 8 ||
		dwVersion > 8 ||
		fFrameRate == NAN ||
		fLastKeyTime == NAN ||
		fFrameRate == 0.0f ||
		skeletonName.empty() ||
		dwBoneCount == 0)
	{
		return false;
	}

	return true;
}
