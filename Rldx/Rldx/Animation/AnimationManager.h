#pragma once

#include "AnimTrackReader.h"
#include "FramePoseGenerator.h"
#include "Skeleton.h"

namespace skel_anim {

	// class AnimationClipManager
	//{
	//	anim_file::AnimFile m_animFile;
	//	FrameGenerator m_framGemmerator

	//};

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

	struct SpliceAnimation
	{
		SkeletonAnimationClip m_clip;
		std::vector<bool> spliceMask;
	};

	class AnimationPlayer
	{
		Skeleton m_skeleton;
		SkeletonAnimationClip m_mainClip;
		std::vector<SpliceAnimation> m_spliceClips;
		std::vector<sm::Matrix> m_framePoseMatrices;

		FramePoseMatrixGenerator m_framePoseGenerator;

	public:
		AnimationPlayer() = default;
		void LoadBindPose(const anim_file::AnimFile& animFile)
		{
			// TODO: put in error checking
			m_skeleton.SetBoneTable(animFile);
			auto bindPoseClip = SkeletonAnimationClip::CreateFromAnimFile(animFile);
			m_skeleton.bindPose = bindPoseClip.frames[0]; // TODO: is this field needed
			m_framePoseGenerator.SetSkeleton(&m_skeleton);
			m_framePoseGenerator.SetAnimCLip(&bindPoseClip);

			// make bindpose matrices
			m_skeleton.bindposeMatrices = m_framePoseGenerator.GenerateFramePoseMatrices();

			// make inverse bindpose matrices
			m_skeleton.inverseBindPoseMatrices.resize(m_skeleton.bindposeMatrices.size());
			for (size_t boneIdex = 0; boneIdex < m_skeleton.bindposeMatrices.size(); boneIdex++)
			{
				m_skeleton.inverseBindPoseMatrices[boneIdex] =
					m_skeleton.bindposeMatrices[boneIdex].Invert();
			}
		}

		void Update(float time)
		{
			m_framePoseMatrices = m_framePoseGenerator.GenerateFramePoseMatrices();
		}

		std::vector<sm::Matrix> GetFramPoseMatrice(float time)
		{
			return m_framePoseMatrices;
		}

		const Skeleton& GetSkeleton()
		{
			return m_skeleton;
		}
	};


} // namespace skel_anim
