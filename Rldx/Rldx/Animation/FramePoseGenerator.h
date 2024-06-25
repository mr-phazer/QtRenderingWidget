#pragma once

#include "AnimTrackReader.h"


namespace skel_anim {

	class Skeleton;
	struct SkeletonAnimation;

	class FramePoseGenerator
	{
		Skeleton m_skeleton;

	public:
		FramePoseGenerator() = default;
		FramePoseGenerator(const Skeleton& skeleton)
			: m_skeleton(skeleton)
		{
		}
		void GenerateMatrices(const SkeletonKeyFrame& frameLocal,
							  std::vector<sm::Matrix>& destPoseMatrices);


		static void GenerateMatrices(const Skeleton& skeleton, const SkeletonKeyFrame& frameLocal,
									 std::vector<sm::Matrix>& destPoseMatrices);

	};


	class FramePoseGeneratorAbstract
	{
	protected:
		const Skeleton* m_pSkeleton = nullptr;

	public:
		/// <summary>
		/// Skeleton'bone table is for "hierachy" when generator global boneTransform	
		/// </summary>		
		void SetSkeleton(const Skeleton* pSkeleton) { m_pSkeleton = pSkeleton; };

		virtual void SetAnimClip(const SkeletonAnimation* pAnimClip) = 0;
		virtual std::vector<sm::Matrix> GenerateFramePoseMatrices(float time = 0.0f) = 0;
	};


	class FramePoseGenerator_PerTrackInterpolation : public FramePoseGeneratorAbstract
	{
		AnimTrackReader m_trackReader;
	public:

		// TODO: maybe make into "getTrackReader()"?
		/// <summary>
		/// Set the current m_animation clip to process
		/// </summary>		
		void SetAnimClip(const SkeletonAnimation* pAnimClip) override
		{
			m_trackReader.SetAnimClip(pAnimClip);
		};

		virtual std::vector<sm::Matrix> GenerateFramePoseMatrices(float time) override;
	};

	class FramePoseGenerator_PerFrameInterpolation : public FramePoseGeneratorAbstract
	{
		const SkeletonAnimation* m_pAnimClip = nullptr;

	private:
		std::vector<sm::Matrix> GenerateDiscreteFramePoseMatrices(size_t frameIndex);

	public:
		virtual void SetAnimClip(const SkeletonAnimation* pAnimClip) override { m_pAnimClip = pAnimClip; }
		virtual std::vector<sm::Matrix> GenerateFramePoseMatrices(float time = 0.0f) override
		{
			// TODO:
				/*
				ALGO:
					- pick frame indexes 0 and frame 0+1 (using) SkeletonAnimation::SystemClockChecker time
					- create interpolated version, again using the time values
					- make global boneTransform matrices



				*/
		}
	};
}