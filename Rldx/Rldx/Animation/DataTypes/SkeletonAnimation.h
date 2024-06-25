#pragma once

#include "..\..\Managers\ResourceManager\IDxResource.h"
#include "Skeleton.h"
#include "SkeletonKeyFrame.h"

namespace anim_file
{
	struct AnimFile;
}

namespace skel_anim
{
	struct SkeletalAnimationKeyFramData
	{
		std::vector<SkeletonKeyFrame> frames;
		std::vector<float> boneBlendWeights; // TODO: for later current anim -> next anim "blend in"?
		std::vector<bool> boneSpliceMask; // TODO: for splicing in other animations, like "hand", "head", "cape", on main "body"-m_animation
	};

	/// <summary>
	/// Contains "relatiave" transforms for each bone in the skeleton.
	/// </summary>
	struct SkeletonAnimation : public rldx::IDxResource
	{
		std::wstring m_skeletonName; // name of the skeleton used by animation

		SkeletalAnimationKeyFramData frameData;

		float lastKeyTime = 0.0f;
		float keyEndTime = 0.0f;
		float keysPerSecond = 0.0f;

	public:
		static SkeletonAnimation* CreateFromAnimFile(const anim_file::AnimFile& in);

		// Inherited via IDxResource, for resource storage stuff
		std::wstring GetTypeString() const override;
		rldx::ResourceTypeEnum GetType() const override;
	};

} // namespace skel_anim