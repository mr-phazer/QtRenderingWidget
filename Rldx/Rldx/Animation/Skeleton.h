#pragma once

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <SimpleMath.h>
#include <vector>

namespace skel_anim
{
	using FramePoseMatrices = std::vector<sm::Matrix>;

	/// <summary>
	/// All the transform info is in the same key, as the TW anim format works that way
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

	struct SkeletonBoneNode
	{
		std::string name = "";
		int32_t boneIndex;
		int32_t parentIndex;
	};

	/// <summary>
	/// Contains keys for all bones for 1 frame	
	/// </summary>
	struct SkeletonKeyFrame
	{
		static SkeletonKeyFrame CreateFromCommonFrame(const anim_file::AnimFrameCommon& input);

		SkeletonKeyFrame() = default;
		SkeletonKeyFrame(size_t boneCount) : boneKeys(boneCount) {}

		std::vector<SimpleBoneKey> boneKeys;
	};

	/// <summary>
	/// Skeleton strucut
	/// Can be used to frame data
	/// </summary>
	struct Skeleton
	{
		Skeleton() = default;
		Skeleton(const anim_file::AnimFile& inputFile)
		{
			SetBoneTable(inputFile);
		}

		void SetBoneTable(const anim_file::AnimFile& inputFile)
		{
			boneTable.clear();
			for (const auto& itBone : inputFile.boneTable.bones)
			{
				SkeletonBoneNode node;

				node.name = itBone.strName;
				node.boneIndex = itBone.id;
				node.parentIndex = itBone.parent_id;

				boneTable.push_back(node);
			}
		}

		FramePoseMatrices inverseBindPoseMatrices;
		FramePoseMatrices bindposeMatrices;

		std::vector<SkeletonBoneNode> boneTable;
		SkeletonKeyFrame bindPose;
	};


	struct SkeletonAnimationClip
	{
		static SkeletonAnimationClip CreateFromAnimFile(const anim_file::AnimFile& in)
		{
			SkeletonAnimationClip newClip;
			newClip.clipLength = in.fileHeader.fEndTime;
			newClip.keysPerSecond = in.fileHeader.fFrameRate;
			for (const auto& frame : in.frames)
			{
				newClip.frames.push_back(SkeletonKeyFrame::CreateFromCommonFrame(frame));
			}

			return newClip;
		}

		std::vector<SkeletonKeyFrame> frames;

		float clipLength = 0.0f;
		float keysPerSecond = 0.0f;
	};

} // namespace skel_anim