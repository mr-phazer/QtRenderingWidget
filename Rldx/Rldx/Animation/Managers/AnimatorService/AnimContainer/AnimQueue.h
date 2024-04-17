#pragma once
#include <memory>
#include <string>
#include <Timer\SystemClockChecker.h>
#include <vector>
#include "AnimContainer.h"


namespace rldx
{
	class DxBaseNode;
	class DxMeshNode;
	class DxDeformerNode;
}

namespace skel_anim
{
	struct SkeletonAnimation;
	struct SkeletonKeyFrame;
	struct SkeletonAnim;
}

namespace skel_anim
{
	struct AnimQueueItem
	{
		AnimQueueItem(const AnimContainer& animSampler);
		AnimQueueItem(SkeletonAnimation* animation);

		AnimContainer anim;
		float timeOffset = 0.0f;
	};

	class AnimQueue
	{
		float m_blendInTime = 0.0f;

		AnimQueueItem* m_currentAnim = nullptr;
		AnimQueueItem* m_nextAnim = nullptr;

		size_t m_loopCount = 0;
		size_t m_oldLoopCount = 0;

		size_t animIndex = 0;
		timer::SystemClockChecker m_clockChecker;
		std::vector<AnimQueueItem> m_animSamplers;

	public:

		/*AnimQueue& operator=(const AnimQueue& source)
		{
			m_animSamplers = source.m_animSamplers;
			return *this;
		}*/

		void AddAnimation(SkeletonAnimation* animation);

		SkeletonKeyFrame Sample(float time);

		float GetLenth() const;

		float GetInterPolationFactor(float time);
		float GetLoopedTime(float time);

		void SwapNextAndCurrent();

		size_t GetNextAnimation();
	};

} // namespace skel_anim