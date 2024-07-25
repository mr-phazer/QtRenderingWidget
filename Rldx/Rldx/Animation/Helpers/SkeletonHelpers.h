#pragma once

#include <string>

// forward declaration
namespace rldx {
	class DxResourceManager;
}

namespace skel_anim
{
	// forward declaration
	class Skeleton;


	extern const std::wstring packSkeletonsPath;
	extern const std::wstring packSkeletonExtention;

	std::wstring GetPackPathFromSkeletonName(const std::wstring& skeletonName);

	Skeleton GetAnimFromFile(rldx::DxResourceManager& resourceManager, const std::wstring& skeletonName);

	void ForceCorrectSkeleton(skel_anim::Skeleton& inOutSkeleton, rldx::DxResourceManager& resourceManager);
};

