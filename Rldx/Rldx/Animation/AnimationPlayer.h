#pragma once

#include <Timer\SystemClockChecker.h>
#include "AnimPlayList.h"
#include "DataTypes\Skeleton.h"
#include "SkeletonKeyFrameSampler.h"

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include "FramePoseGenerator.h"

namespace skel_anim {



	// TODO: clean up
	//class AnimationPlayer_Old
	//{
	//	timer::SystemClockChecker m_timer;
	//	Skeleton m_skeleton;
	//	SkeletonAnimation* m_animation = nullptr;
	//	std::vector<sm::Matrix> m_framePoseMatrices;

	//	std::unique_ptr<FramePoseGeneratorAbstract> m_framePoseGenerator = std::make_unique<FramePoseGenerator_PerTrackInterpolation>();

	//public:
	//	void CreateBindPose(const ::anim_file::AnimFile& animFile)
	//	{
	//		// TODO: put in error checking
	//		m_skeleton.SetBoneTable(animFile);
	//		m_animation = SkeletonAnimation::CreateFromAnimFile(animFile);

	//		m_skeleton.bindPose = m_animation->frameData.frames[0]; // TODO: is this field needed

	//		m_framePoseGenerator->SetSkeleton(&m_skeleton);
	//		m_framePoseGenerator->SetAnimClip(m_animation);

	//		// make bindpose matrices
	//		m_skeleton.m_bindposeMatrices = m_framePoseGenerator->GenerateFramePoseMatrices();

	//		// make inverse bindpose matrices
	//		m_skeleton.m_inverseBindPoseMatrices.resize(m_skeleton.m_bindposeMatrices.size());
	//		for (size_t boneIdex = 0; boneIdex < m_skeleton.m_bindposeMatrices.size(); boneIdex++)
	//		{
	//			m_skeleton.m_inverseBindPoseMatrices[boneIdex] =
	//				m_skeleton.m_bindposeMatrices[boneIdex].Invert();
	//		}
	//	}

	//	void SetAnimation(const anim_file::AnimFile& animFile)
	//	{
	//		m_animation = SkeletonAnimation::CreateFromAnimFile(animFile);
	//		m_framePoseGenerator->SetAnimClip(m_animation);
	//	}

	//	void Update(float time)
	//	{
	//		if (m_skeleton.m_bindposeMatrices.empty()) {
	//			return;
	//		}

	//		// TODO: use the input time value? Some "global" time management needed ?
	//		// TODO: CHANGFE BACK ONCE WORKS
	//		m_framePoseMatrices = m_framePoseGenerator->GenerateFramePoseMatrices(time);
	//	}

	//	const std::vector<sm::Matrix>& GetFramPoseMatrices()
	//	{
	//		return m_framePoseMatrices;
	//	}

	//	const std::vector<sm::Matrix>& GetInverseBindPoseMatrices() const
	//	{
	//		return m_skeleton.m_inverseBindPoseMatrices;
	//	}

	//	const Skeleton& GetSkeleton()
	//	{
	//		return m_skeleton;
	//	}
	//};

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

		void CreateBindPose(const anim_file::AnimFile& animFile)
		{
			// TODO: put in error checking

			//m_animation = SkeletonAnimation::CreateFromAnimFile(animFile);

			//// make bindpose matrices
			//FramePoseMatrices bindposeMatrices;
			//FramePoseGenerator(m_skeleton).GenerateMatrices(m_animation->frameData.frames[0], bindposeMatrices);
			//m_skeleton.m_bindposeMatrices = bindposeMatrices;
			m_skeleton = Skeleton(animFile);

			// TODO: remove if above works
			//// make inverse bindpose matrices
			//m_skeleton.m_inverseBindPoseMatrices.resize(m_skeleton.m_bindposeMatrices.size());
			//for (size_t boneIdex = 0; boneIdex < m_skeleton.m_bindposeMatrices.size(); boneIdex++)
			//{
			//	m_skeleton.m_inverseBindPoseMatrices[boneIdex] =
			//		m_skeleton.m_bindposeMatrices[boneIdex].Invert();
			//}
		}

		void SetAnimation(const anim_file::AnimFile& animFile)
		{
			auto animation = SkeletonAnimation::CreateFromAnimFile(animFile);
			m_animationSamplers->SetAnimation(animation);
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
