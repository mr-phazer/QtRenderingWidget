#include <stdlib.h>
#include "..\Rldx\Rldx\Managers\DxDeviceManager.h"
#include "AnimPlayList.h"
#include "KeyFrameInterpolator.h"

namespace skel_anim
{
	float SkeletonAnimQueueBlendampler::GetInterPolationFactor(float time)
	{
		auto blendStart1 = m_currentAnim->GetLenth() - m_blendInTime;
		auto blendStart2 = blendStart1;

		if (time < blendStart1) // if time is before blend start
		{
			// only for debugging
			//rldx::DxDeviceManager::DebugTextWriter()->SetStringRow(0, L"Time:" + to_wstring(time) + L" / " + to_wstring(GetLenth()) + L": " + to_wstring((uint64_t)(m_currentAnim)) + L"Interpolation factor : 0.0");
			return 0.0f;
		}
		else if (time > blendStart2)
		{
			// only for debugging
			//rldx::DxDeviceManager::DebugTextWriter()->SetStringRow(1, L"BLENDING: Time:" + to_wstring(time) + L" / " + to_wstring(GetLenth()) + L": " + to_wstring((uint64_t)(m_currentAnim)) + L"Interpolation factor : " + std::to_wstring((time - blendStart1) / m_blendInTime));
			return (time - blendStart1) / m_blendInTime;
		}

		return 0.0f;
	}

	float SkeletonAnimQueueBlendampler::GetLoopedTime(float time)
	{
		m_loopCount = static_cast<size_t>(time / GetLenth()); // number of times that anim could have run
		float correctedTime = time - m_loopCount * GetLenth(); // correct time, to [0; fAnimEnd] ("loop anim")

		return correctedTime;
	}

	void SkeletonAnimQueueBlendampler::SwapNextAndCurrent()
	{
		//rldx::DxDeviceManager::DebugTextWriter()->SetStringRow(2, L"SWAP!!!");

		// swap animations		
		m_currentAnim = m_nextAnim;
		m_nextAnim = &m_animSamplers[GetNextAnimation()];

		// swap offsets		
		m_currenOffest = m_blendInTime;
		m_nextOffest = -(m_currentAnim->GetLenth() - m_blendInTime);;
	}

	size_t SkeletonAnimQueueBlendampler::GetNextAnimation()
	{
		srand(static_cast<unsigned int>(m_clockChecker.GetSystemTick()));
		animIndex = rand() % m_animSamplers.size();

		if (animIndex >= m_animSamplers.size()) { animIndex = 0; }

		return animIndex;
	}

	void SkeletonAnimQueueBlendampler::SetAnimation(SkeletonAnimation* m_animation)
	{
		// TODO: this mess is only for testing		
		// TODO: simple set both current and next anims to "GetNextAnimation()"

		m_animSamplers.push_back(SkeletalAnimationClipSampler(m_animation));

		if (m_animSamplers.size() >= 2)
		{
			m_currentAnim = &m_animSamplers[GetNextAnimation()];
			m_nextAnim = &m_animSamplers[GetNextAnimation()];

			m_currenOffest = 0.0f;
			m_nextOffest = -(m_currentAnim->GetLenth() - m_blendInTime);
		}
	}

	SkeletonKeyFrame SkeletonAnimQueueBlendampler::Sample(float time)
	{
		auto loopedTime = m_clockChecker.GetLocalTime();

		if (loopedTime > m_currentAnim->GetLenth())
		{
			SwapNextAndCurrent();
			m_clockChecker.ResetLocalTime();

			loopedTime = m_clockChecker.GetLocalTime();

			// TODO: remove debuggind code
			auto debug_new_current_len = m_currentAnim->GetLenth();
			auto DEBUG_BREAK_1 = 1;
		}

		// TODO: remove debuggind code
		// TODO: this block is only for testing
		if (loopedTime > m_currentAnim->GetLenth() - m_blendInTime)
		{
			auto DEBUG_BREAK_1 = 1;
		}

		return KeyFrameInterpolator::InterpolateLocalFrames(
			m_currentAnim->Sample(loopedTime + m_currenOffest),
			m_nextAnim->Sample(loopedTime + m_nextOffest),
			GetInterPolationFactor(loopedTime));
	}

	float SkeletonAnimQueueBlendampler::GetLenth() const
	{
		return static_cast<float>(m_currentAnim->GetLenth() + m_nextAnim->GetLenth());
	}
}