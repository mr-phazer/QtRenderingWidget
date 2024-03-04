#pragma once

#include <SimpleMath.h>
#include <vector>

namespace animation
{
	struct KeyFrame
	{
		KeyFrame() = default;
		KeyFrame(float time, sm::Vector3 position, sm::Quaternion rotation, sm::Vector3 scale)
			:
			time(time),
			position(position),
			rotation(rotation),
			scale(scale) {}


		float time = 0.0f;
		sm::Vector3 position = { 0,0,0 };
		sm::Quaternion rotation = sm::Quaternion::Identity;
		sm::Vector3 scale = { 0,0,0 };;
	};

	struct SkeletonBoneNode
	{
		int32_t boneIndex;
		int32_t parentIndex;
		KeyFrame transform;
	};

	struct Skeleton
	{
		std::vector<sm::Matrix> inverseBindPoseMatrices;
		std::vector<SkeletonBoneNode> bones;
	};


	struct AnimationFrame
	{
		std::vector<KeyFrame> skeletonNodes;
	};

	struct Animation
	{
		std::vector<AnimationFrame> frames;
	};

	struct AnimationClip
	{
		Animation animation;
		float lenth = 0.0f

	};

} // namespace animation