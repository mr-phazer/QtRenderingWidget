#pragma once

#include <string>

namespace skel_anim
{
	// forward declaration
	class Skeleton;

	extern const std::wstring packSkeletonsPath;
	extern const std::wstring packSkeletonExtention;

	std::wstring GetPackPathFromSkeletonName(const std::wstring& skeletonName);

	Skeleton GetAnimFromFile(const std::wstring& skeletonName);

	void ForceCorrectSkeleton(skel_anim::Skeleton& inOutSkeleton);
};

