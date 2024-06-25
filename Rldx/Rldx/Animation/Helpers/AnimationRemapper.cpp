#include <ImportExport\FileFormats\Anim\Reader\TwAnimReader.h>
#include <ImportExport\helpers\ByteStream.h>
#include "AnimationRemapper.h"
#include "SkeletonHelpers.h"

namespace skel_anim
{
	AnimationRemapper::AnimationRemapper(const std::wstring& sourceSkeletonPath, const std::wstring& targetSkeletonPath)
	{
		m_sourceSkeleton = GetAnimFromFile(sourceSkeletonPath);
		m_targetSkeleton = GetAnimFromFile(targetSkeletonPath);
	}

	SkeletonAnimation AnimationRemapper::RemapAnimation(const SkeletonAnimation& sourceAnim)
	{
		SkeletonAnimation targetAnim;
		// resize the targetAnim m_animation to match the targetAnim skeleton
		targetAnim.frameData.frames.resize(sourceAnim.frameData.frames.size());

		// run through sourceAnim frames, and copy them to the targetAnim m_animation
		for (size_t i = 0; i < sourceAnim.frameData.frames.size(); i++)
		{
			targetAnim.frameData.frames[i] = RemapFrame(sourceAnim.frameData.frames[i]);
		}

		return targetAnim;
	}

	SkeletonKeyFrame AnimationRemapper::RemapFrame(const SkeletonKeyFrame& sourceFrame)
	{
		// resize the targetAnim sourceFrame to match the targetAnim skeleton
		SkeletonKeyFrame targetFreame;
		targetFreame.boneKeys.resize(m_targetSkeleton.GetBoneTable().size());

		// run through sourceAnim bones tracks, and copy them to the targetAnim sourceFrame
		for (auto& srcBone : m_sourceSkeleton.GetBoneTable())
		{
			// get the targetAnim bone index
			auto targetBone = m_targetSkeleton.GetIndexFromBoneName(srcBone.name);

			if (targetBone == -1)
				throw std::exception("Bone not found in the target skeleton");

			// copy sourceAnim key to the targetAnim sourceFrame key
			targetFreame.boneKeys[targetBone] = sourceFrame.boneKeys[srcBone.boneIndex];
		}

		return targetFreame;
	}
}