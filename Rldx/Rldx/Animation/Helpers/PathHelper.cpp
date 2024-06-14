
#include <Rldx\Tools\tools.h>
#include "PathHelper.h"

namespace path_helper
{
	std::wstring GetPathFromSkeletonName(const std::string& skeletonName)
	{
		return LR"(animations\skeletons\)" + ToWString(skeletonName) + L".anim";
	};
}