#pragma once

#include <CommonLibs\Timer\SystemClockChecker.h>
#include "AnimPlayList.h"
#include "DataTypes\Skeleton.h"
#include "SkeletonKeyFrameSampler.h"

#include <ImportExport\FileFormats\Anim\Types\Common\TwAnimFile.h>
#include "FramePoseGenerator.h"

namespace skel_anim
{

	class AnimationPlayer
	{
		timer::SystemClockChecker m_timer;
		Skeleton m_skeleton;
		SkeletonAnimation* m_animation;

		std::vector<sm::Matrix> m_framePoseMatrices;
		std::shared_ptr<ISkeletalAnimationSampler> m_animationSamplers = std::make_shared<SkeletonAnimQueueBlendampler>();

	public:
		AnimationPlayer& operator= (AnimationPlayer& other)
		{
			m_skeleton = other.m_skeleton;

			m_animationSamplers = std::make_shared<SkeletonAnimQueueBlendampler>();

			*static_cast<SkeletonAnimQueueBlendampler*>(m_animationSamplers.get()) =
				*static_cast<SkeletonAnimQueueBlendampler*>(other.m_animationSamplers.get());

			return *this;
		};

		void CreateBindPose(rldx::DxResourceManager& resourceManager, const anim_file::AnimFile& animFile)
		{
			// TODO: put in error checking

			//m_animation = SkeletonAnimation::CreateFromAnimFile(animFile);

			//// make bindpose matrices
			//FramePoseMatrices bindposeMatrices;
			//FramePoseGenerator(m_skeleton).GenerateMatrices(m_animation->frameData.frames[0], bindposeMatrices);
			//m_skeleton.m_bindposeMatrices = bindposeMatrices;
			m_skeleton = Skeleton(resourceManager, animFile);

			// TODO: remove if above works
			//// make inverse bindpose matrices
			//m_skeleton.m_inverseBindPoseMatrices.resize(m_skeleton.m_bindposeMatrices.size());
			//for (size_t boneIdex = 0; boneIdex < m_skeleton.m_bindposeMatrices.size(); boneIdex++)
			//{
			//	m_skeleton.m_inverseBindPoseMatrices[boneIdex] =
			//		m_skeleton.m_bindposeMatrices[boneIdex].Invert();
			//}
		}

		// TODO: rename, to "Load"/similar, "set" sounds like the input only need to be copied
		void SetAnimation(rldx::DxResourceManager& resourceManager, const anim_file::AnimFile& animFile)
		{
			auto m_animation = SkeletonAnimation::CreateFromAnimFile(resourceManager, animFile);
			m_animationSamplers->SetAnimation(m_animation);
		}

		void Update(float time)
		{
			auto keyFrame = m_animationSamplers->Sample(time);
			FramePoseGenerator(m_skeleton).GenerateMatrices(keyFrame, m_framePoseMatrices);
		}

		const std::vector<sm::Matrix>& GetFramPoseMatrices()
		{
			return m_framePoseMatrices;
		}

		const std::vector<sm::Matrix>& GetInverseBindPoseMatrices() const
		{
			return m_skeleton.GetInverseBindPoseMatrices();
		}

		const Skeleton& GetSkeleton()
		{
			return m_skeleton;
		}
	};

} // namespace skel_anim
