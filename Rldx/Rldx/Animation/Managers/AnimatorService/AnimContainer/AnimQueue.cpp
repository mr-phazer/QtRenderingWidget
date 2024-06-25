#include <Rldx\Managers\DxDeviceManager.h>
#include "..\..\..\DataTypes\SkeletonKeyFrame.h"
#include "..\..\..\KeyFrameInterpolator.h"
#include "AnimContainer.h"
#include "AnimQueue.h"

namespace skel_anim
{

	float AnimQueue::GetInterPolationFactor(float time)
	{
		auto blendStart1 = m_currentAnim->anim.GetLenth() - m_blendInTime;

		if (time < blendStart1) // if time is before blend start
		{
			// TODO: debugging onlye
			// dx::DxDeviceManager::DebugTextWriter()->SetStringRow(0, L"Time:" + to_wstring(time) + L" / " + to_wstring(GetLenth()) + L/Interpolation factor : 0.0");
			return 0.0f;
		}
		else if (time > blendStart1)

			// TODO: debugging onlye{
			//rldx::DxDeviceManager::DebugTextWriter()->SetStringRow(1, L"BLENDING: Time:" + to_wstring(time - blendStart1) + L" / " + to_wstring(GetLenth()) + L"Interpolation factor : " + std::to_wstring((time - blendStart1) / m_blendInTime));
			return (time - blendStart1) / m_blendInTime;


		auto debug_break = 1;
		return 0.0f;
	}

	//float AnimQueue::GetLoopedTime(float time)
	//{
	//	m_loopCount = static_cast<size_t>(time / GetLenth()); // number of times that anim could have run
	//	float correctedTime = time - m_loopCount * GetLenth(); // correct time, to [0; fAnimEnd] ("loop anim")

	//	return correctedTime;
	//}

	void AnimQueue::SwapNextAndCurrent()
	{


		// TODO: only for debugging
		// auto DEBUG_STOP_TIME = m_clockChecker.GetLocalTime();
		//rldx::DxDeviceManager::DebugTextWriter()->SetStringRow(2, L"SWAP!!!");

		// - Interpolation/swap playlisting:

		/*                B    A
		-----------------------
						  |----|-------------------
							   B			  |----|-------------------
		*/
		// swap animations		
		m_currentAnim = m_nextAnim;
		m_nextAnim = &m_animSamplers[GetNextAnimation()];

		// swap offsets		
		m_currentAnim->timeOffset = m_blendInTime;
		m_nextAnim->timeOffset = -(m_currentAnim->anim.GetLenth() - m_blendInTime);
	}

	size_t AnimQueue::GetNextAnimation()
	{
		srand(static_cast<unsigned int>(m_clockChecker.GetSystemTick()));
		animIndex = rand() % m_animSamplers.size();

		if (animIndex >= m_animSamplers.size()) { animIndex = 0; }

		return animIndex;
	}

	void AnimQueue::AddAnimation(SkeletonAnimation* m_animation)
	{
		// TODO: this mess is only for testing		
		// TODO: simple set both current and next anims to "GetNextAnimation()"		

		m_animSamplers.push_back(AnimQueueItem(m_animation));

		if (m_animSamplers.size() >= 1)
		{
			m_currentAnim = &m_animSamplers[GetNextAnimation()];
			m_nextAnim = &m_animSamplers[GetNextAnimation()];

			m_currentAnim->timeOffset = 0.0f;
			m_nextAnim->timeOffset = -(m_currentAnim->anim.GetLenth() - m_blendInTime);
		}
	}

	SkeletonKeyFrame AnimQueue::Sample(float time)
	{
		auto loopedTime = m_clockChecker.GetLocalTime();

		if (loopedTime + m_currentAnim->timeOffset >= m_currentAnim->anim.GetLenth())
		{

			auto DEBUG_STOP = loopedTime + m_nextAnim->timeOffset;
			auto DEBUG_loopedTime = loopedTime;
			SwapNextAndCurrent();
			m_clockChecker.ResetLocalTime();

			loopedTime = m_clockChecker.GetLocalTime();

			// TODO: remove debuggind code
			auto debug_new_current_len = m_currentAnim->anim.GetLenth();
			auto DEBUG_BREAK_1 = 1;
		}

		// TODO: remove debuggind code
		// TODO: this block is only for testing
		if (loopedTime > m_currentAnim->anim.GetLenth() - m_blendInTime)
		{
			auto DEBUG_BREAK_1 = 1;
		}



		return KeyFrameInterpolator::InterpolateLocalFrames(
			m_currentAnim->anim.Sample((float)loopedTime + m_currentAnim->timeOffset),
			m_nextAnim->anim.Sample((float)loopedTime + m_nextAnim->timeOffset),
			GetInterPolationFactor((float)loopedTime + m_currentAnim->timeOffset));
	}

	float AnimQueue::GetLenth() const
	{
		return static_cast<float>(m_currentAnim->anim.GetLenth() + m_nextAnim->anim.GetLenth());
	}

	AnimQueueItem::AnimQueueItem(const AnimContainer& animSampler) : anim(animSampler) {}

	AnimQueueItem::AnimQueueItem(SkeletonAnimation* m_animation)
	{
		anim = AnimContainer(m_animation);
	}

} // namespace skel_anim
