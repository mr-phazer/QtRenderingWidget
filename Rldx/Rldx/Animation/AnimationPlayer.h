#pragma once

#include <Timer\SystemClockChecker.h>
#include "..\\Rendering\DxConstBuffer.h"
#include "AnimTrackReader.h"
#include "FramePoseGenerator.h"
#include "Skeleton.h"

namespace skel_anim {

	class FrameInterPolator
	{
	public:
		static SkeletonKeyFrame InterpolateLocalFrames(const SkeletonKeyFrame& frame0,
													   const SkeletonKeyFrame& frame1,
													   float blendFactor);
	};

	class FrameGenerator
	{
		Skeleton m_skeleton;

	public:
		FrameGenerator(Skeleton& skeleton)
			: m_skeleton(skeleton)
		{
		}
		void GeneateFramePoseMatrices(const SkeletonKeyFrame& frameLocal,
									  std::vector<sm::Matrix> destPoseMatrices);
	};

	class AnimationPlayer
	{
		timer::SystemClockChecker m_timer;
		Skeleton m_skeleton;
		SkeletonAnimation m_animation;
		std::vector<sm::Matrix> m_framePoseMatrices;

		std::unique_ptr<FramePoseGeneratorAbstract> m_framePoseGenerator = std::make_unique<FramePoseGenerator_PerTrackInterpolation>();

	public:
		void CreateBindPose(const anim_file::AnimFile& animFile)
		{
			// TODO: put in error checking
			m_skeleton.SetBoneTable(animFile);
			m_animation = SkeletonAnimation::CreateFromAnimFile(animFile);

			m_skeleton.bindPose = m_animation.frameData.frames[0]; // TODO: is this field needed

			m_framePoseGenerator->SetSkeleton(&m_skeleton);
			m_framePoseGenerator->SetAnimClip(&m_animation);

			// make bindpose matrices
			m_skeleton.bindposeMatrices = m_framePoseGenerator->GenerateFramePoseMatrices();

			// make inverse bindpose matrices
			m_skeleton.inverseBindPoseMatrices.resize(m_skeleton.bindposeMatrices.size());
			for (size_t boneIdex = 0; boneIdex < m_skeleton.bindposeMatrices.size(); boneIdex++)
			{
				m_skeleton.inverseBindPoseMatrices[boneIdex] =
					m_skeleton.bindposeMatrices[boneIdex].Invert();
			}
		}

		void SetAnimation(const anim_file::AnimFile& animFile)
		{
			m_animation = SkeletonAnimation::CreateFromAnimFile(animFile);
			m_framePoseGenerator->SetAnimClip(&m_animation);
		}

		void Update(float time)
		{
			if (m_skeleton.bindposeMatrices.empty()) {
				return;
			}

			// TODO: use the input time value? Some "global" time management needed ?
			// TODO: CHANGFE BACK ONCE WORKS
			m_framePoseMatrices = m_framePoseGenerator->GenerateFramePoseMatrices(time);
		}

		const std::vector<sm::Matrix>& GetFramPoseMatrices()
		{
			return m_framePoseMatrices;
		}

		const std::vector<sm::Matrix>& GetInverseBindPoseMatrices() const
		{
			return m_skeleton.inverseBindPoseMatrices;
		}

		const Skeleton& GetSkeleton()
		{
			return m_skeleton;
		}
	};


} // namespace skel_anim
