#pragma once

#include <DirectXTK\Inc\SimpleMath.h>
#include <vector>

namespace anim_file
{
	struct AnimFrameCommon;
}

namespace skel_anim
{
	/// <summary>
	/// All the boneTransform info is in the same key, as the TW anim format works that way
	/// </summary>
	struct SimpleBoneKey
	{
		SimpleBoneKey() = default;
		SimpleBoneKey(float time, sm::Vector3 position, sm::Quaternion rotation, sm::Vector3 scale)
			:
			time(time),
			translation(position),
			rotation(rotation),
			scale(scale) {}

		float time = 0.0f;
		sm::Vector3 translation = { 0,0,0 };
		sm::Quaternion rotation = sm::Quaternion::Identity;
		sm::Vector3 scale = { 0,0,0 };

		sm::Matrix GetTransForm() const;
	};

	/// <summary>
	/// Contains keys for all bones for 1 frame	
	/// </summary>
	struct SkeletonKeyFrame
	{
		std::vector<SimpleBoneKey> boneKeys;

		static SkeletonKeyFrame CreateFromCommonFrame(const anim_file::AnimFrameCommon& input);

		SkeletonKeyFrame() = default;
		SkeletonKeyFrame(size_t boneCount) : boneKeys(boneCount) {}
	};
}