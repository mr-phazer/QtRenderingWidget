#pragma once

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <SimpleMath.h>
#include "..\..\Tools\tools.h"
#include "SkeletonKeyFrame.h"

namespace skel_anim
{
	using FramePoseMatrices = std::vector<sm::Matrix>;

	/// <summary>
	/// Used to define the parent-child hierachy of a skeleton
	/// </summary>
	struct SkeletonBoneNode
	{
		std::string name = "";
		int32_t boneIndex = -1;
		int32_t parentIndex = -1;

		std::vector<SkeletonBoneNode*> children;
	};

	/// <summary>
	/// Skeleton strucut
	/// Can be used to frame data
	/// </summary>
	struct Skeleton
	{
		const static int32_t NO_PARENT = -1;
		Skeleton() = default;
		Skeleton& operator=(Skeleton& inputFile);

		Skeleton(const anim_file::AnimFile& inputFile);

		void SetBoneTable(const anim_file::AnimFile& inputFile);

		int32_t GetIndexFromBoneName(const std::string& boneName) const;

		FramePoseMatrices inverseBindPoseMatrices;
		FramePoseMatrices bindposeMatrices;

		std::vector<SkeletonBoneNode> boneTable;
		SkeletonKeyFrame bindPose;
	};


} // namespace skel_anim