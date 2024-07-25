#pragma once

#include "..\DataTypes\SkeletonAnimation.h"

namespace skel_anim
{
	class AnimationRemapper
	{
		Skeleton m_sourceSkeleton;
		Skeleton m_targetSkeleton;

	public:
		AnimationRemapper(rldx::DxResourceManager& resourceManager, const std::wstring& sourceSkeletonPath, const std::wstring& targetSkeletonPath);
		SkeletonAnimation RemapAnimation(const SkeletonAnimation& source);

	private:
		SkeletonKeyFrame RemapFrame(const SkeletonKeyFrame& source);
	};
}; // namespace skel_anim
