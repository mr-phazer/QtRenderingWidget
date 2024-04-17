#include "KeyFrameInterpolator.h"
#include "SkeletonKeyFrameSampler.h"

namespace skel_anim
{
	SkeletonKeyFrame SkeletalAnimationClipSampler::Sample(float time)
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
} // namespace skel_anim
