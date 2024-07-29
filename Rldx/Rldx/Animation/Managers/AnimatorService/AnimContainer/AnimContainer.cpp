#include <Rldx/Rldx/Animation/Helpers/timerHelper.h>
#include <Rldx/Rldx/Animation/KeyFrameInterpolator.h>

#include "AnimContainer.h"

namespace skel_anim
{
	void AnimContainer::SetAnimation(SkeletonAnimation* m_animation)
	{
		m_animation = m_animation;
	}

	SkeletonKeyFrame AnimContainer::Sample(float time)
	{
		auto TimeValues = AnimationTimerHelper::GetLoopedTimeValues(
			time,
			(1.0f / m_animation->keysPerSecond) * (m_animation->frameData.frames.size()),
			m_animation->keysPerSecond);

		return KeyFrameInterpolator::InterpolateLocalFrames(
			m_animation->frameData.frames[TimeValues.frameIndex0],
			m_animation->frameData.frames[TimeValues.frameIndex1],
			TimeValues.interpolationFactor);
	}

	float AnimContainer::GetLenth() const {
		return (1.0f / m_animation->keysPerSecond) * static_cast<float>(m_animation->frameData.frames.size());
	}

}
