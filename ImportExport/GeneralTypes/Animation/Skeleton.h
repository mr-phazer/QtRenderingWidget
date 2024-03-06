#pragma once

#include <SimpleMath.h>
#include <vector>

namespace skel_anim
{
	struct BoneKey
	{
		BoneKey() = default;
		BoneKey(float time, sm::Vector3 position, sm::Quaternion rotation, sm::Vector3 scale)
			:
			time(time),
			translation(position),
			rotation(rotation),
			scale(scale) {}


		float time = 0.0f;
		sm::Vector3 translation = { 0,0,0 };
		sm::Quaternion rotation = sm::Quaternion::Identity;
		sm::Vector3 scale = { 0,0,0 };;
	};

	struct SkeletonBoneNode
	{
		int32_t boneIndex;
		int32_t parentIndex;
	};

	struct SkeletonFrame
	{
		std::vector<BoneKey> boneKeys;
	};

	/// <summary>
	/// Skeleton strucut
	/// Can be used to frame data
	/// </summary>
	struct Skeleton
	{
		std::vector<sm::Matrix> inverseBindPoseMatrices;
		std::vector<SkeletonBoneNode> bones;
		SkeletonFrame bindPose;
	};

	struct SkeletonAnimationData
	{
		std::vector<SkeletonFrame> frames;
	};

	struct SkeletonAnimationClip
	{
		SkeletonAnimationData animationData;
		float clipLength = 0.0f;
		float keysPerSecond = 0.0f;
	};

} // namespace skel_anim