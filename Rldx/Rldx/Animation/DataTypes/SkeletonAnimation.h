#pragma once

#include "..\..\Managers\ResourceManager\DxResourceManager.h"
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
		SkeletonAnimation() = default;
		SkeletonAnimation(const std::wstring& name)

			// TODO: corred back if IDxResource gets a new constructor
			// :
			//rldx::IDxResource(rldx::ResourceTypeEnum::Animation, L"SkeletonAnimation", name)
		{};

		std::wstring m_skeletonName; // m_nodeName of the skeleton used by animation

		SkeletalAnimationKeyFramData frameData;

		float lastKeyTime = 0.0f;
		float keyEndTime = 0.0f;
		float keysPerSecond = 0.0f;

	public:
		static SkeletonAnimation* CreateFromAnimFile(rldx::DxResourceManager& resoureceMangager, const anim_file::AnimFile& inAnimFile);

	};
} // namespace skel_anim