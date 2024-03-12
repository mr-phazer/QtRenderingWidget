#include "FileHeader.h"

inline bool anim_file::AnimHeaderCommon::IsContentValid()
{
	if (
		dwVersion > 8 ||
		fFrameRate == NAN ||
		fFrameRate == 0.0f ||
		skeletonName.empty() ||
		dwBoneCount == 0

		)
		return false;

	return true;
}
