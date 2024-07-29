#pragma once

#include <CommonLibs\Timer\SystemClockChecker.h>
#include "SkeletonKeyFrameSampler.h"

namespace skel_anim
{
	class SkeletonAnimQueueBlendampler : public ISkeletalAnimationSampler
	{

		float m_blendInTime = 0.5f;

		SkeletalAnimationClipSampler* m_currentAnim = nullptr;
		SkeletalAnimationClipSampler* m_nextAnim = nullptr;

		float m_currenOffest = 0.0f;
		float m_nextOffest = 0.0f;
		size_t m_loopCount = 0;
		size_t m_oldLoopCount = 0;

		float GetInterPolationFactor(float time);
		float GetLoopedTime(float time);

		void SwapNextAndCurrent();

		size_t GetNextAnimation();

		size_t animIndex = 0;
		timer::SystemClockChecker m_clockChecker;
		std::vector<SkeletalAnimationClipSampler> m_animSamplers;

	public:
		SkeletonAnimQueueBlendampler& operator=(const SkeletonAnimQueueBlendampler& source)
		{
			m_animSamplers = source.m_animSamplers;
			return *this;
		}

		void SetAnimation(SkeletonAnimation* m_animation) override;

		SkeletonKeyFrame Sample(float time) override;

		float GetLenth() const override;
	};
}