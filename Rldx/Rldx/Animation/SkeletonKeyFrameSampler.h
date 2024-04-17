#pragma once	

#include "DataTypes\SkeletonAnimation.h"
#include "Helpers\timerHelper.h"

namespace skel_anim
{

	class ISkeletalAnimationSampler
	{
	public:
		virtual void SetAnimation(SkeletonAnimation* animation) = 0;
		virtual SkeletonKeyFrame Sample(float time) = 0;
		virtual float GetLenth() const = 0;
	};

	class SkeletalAnimationClipSampler : public ISkeletalAnimationSampler
	{
		SkeletonAnimation* m_animation;

	public:
		SkeletalAnimationClipSampler() = default;
		SkeletalAnimationClipSampler(SkeletonAnimation* animation) : m_animation(animation) {}

		void SetAnimation(SkeletonAnimation* animation) override
		{
			m_animation = animation;
		}

		SkeletonKeyFrame Sample(float time) override;

		float GetLenth() const  override {
			return (1.0f / m_animation->keysPerSecond) * static_cast<float>(m_animation->frameData.frames.size());
		}
	};
}