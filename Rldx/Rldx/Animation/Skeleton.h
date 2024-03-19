#pragma once

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <SimpleMath.h>
#include <Timer\SystemClockChecker.h>
#include <vector>
#include "..\Tools\tools.h"


namespace skel_anim
{
	using FramePoseMatrices = std::vector<sm::Matrix>;

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
	/// Used to define the parent-child hierachy of a skeleton
	/// </summary>
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
		std::vector<SimpleBoneKey> boneKeys;

		static SkeletonKeyFrame CreateFromCommonFrame(const anim_file::AnimFrameCommon& input);

		SkeletonKeyFrame() = default;
		SkeletonKeyFrame(size_t boneCount) : boneKeys(boneCount) {}
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


	struct SkeletonKeyFrameAnimationData
	{
		std::vector<SkeletonKeyFrame> frames;
		std::vector<float> boneBlendWeights; // TODO: for later current anim -> next anim "blend in"?
		std::vector<bool> boneSpliceMask; // TODO: for splicing in other animations, like "hand", "head", "cape", on main "body"-animation
	};

	/// <summary>
	/// Contains "relatiave
	/// </summary>
	struct SkeletonAnimation
	{
		timer::SystemClockChecker timer;
		SkeletonKeyFrameAnimationData frameData;

		float lastKeyTime = 0.0f;
		float keyEndTime = 0.0f;
		float keysPerSecond = 0.0f;

	public:
		static SkeletonAnimation CreateFromAnimFile(const anim_file::AnimFile& in)
		{
			SkeletonAnimation newAnim;
			newAnim.lastKeyTime = in.fileHeader.fLastKeyTime;
			newAnim.keysPerSecond = in.fileHeader.fFrameRate;

			for (const auto& frame : in.frames)
			{
				newAnim.frameData.frames.push_back(SkeletonKeyFrame::CreateFromCommonFrame(frame));
				newAnim.frameData.boneBlendWeights.push_back(1.0f);
				newAnim.frameData.boneSpliceMask.push_back(true);
			}

			return newAnim;
		}

		const SkeletonKeyFrame& GetInterpolatedLocalFrame() // TODO: should this be here, or in "FramePoseGenerator"??
		{

		}
	};

} // namespace skel_anim