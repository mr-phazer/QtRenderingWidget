#pragma once

#include <SimpleMath.h>
#include <vector>
#include "..\..\FileFormats\Anim\Types\Common\TwAnimFile.h"
#include "Skeleton.h"


namespace skel_anim
{
	class AnimTrackReader
	{
		const SkeletonAnimationClip* pAnimClip = nullptr;
		float m_currentTime = 0.0f;

	public:
		AnimTrackReader() = default;

		void SetAnimClip(const SkeletonAnimationClip* animClip) { pAnimClip = animClip; };

		float SetTime(float time) { m_currentTime = time; }

		sm::Vector3 GetTranslationDiscrete(size_t boneIndex, size_t frmaeIndex) const
		{
			return pAnimClip->frames.at(frmaeIndex).boneKeys.at(boneIndex).translation;
		}

		sm::Quaternion GetRotationDiscrete(size_t boneIndex, size_t frmaeIndex) const
		{
			return pAnimClip->frames.at(frmaeIndex).boneKeys.at(boneIndex).rotation;
		}

		sm::Vector3 GetTranslation(size_t boneIndex) const
		{
			size_t loopCount = static_cast<size_t>(m_currentTime / pAnimClip->clipLength); // number of time that anim could have tun
			float correctedTime = m_currentTime - loopCount * pAnimClip->clipLength; // correct time, to it in [0; fAnimEnd]

			size_t frameIndex = static_cast<size_t>(correctedTime * pAnimClip->keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
			float delta = correctedTime - static_cast<float>(frameIndex); // distance to next frame

			const auto& translation0 = GetTranslationDiscrete(boneIndex, frameIndex);
			const auto& translation1 = (correctedTime >= pAnimClip->clipLength) ?
				GetTranslationDiscrete(boneIndex, 0) : GetTranslationDiscrete(boneIndex, frameIndex);

			return sm::Vector3::Lerp(translation0, translation1, delta);
		}

		sm::Quaternion GetRotation(size_t boneIndex) const
		{
			size_t loopCount = static_cast<size_t>(m_currentTime / pAnimClip->clipLength); // number of time that anim could have tun
			float correctedTime = m_currentTime - loopCount * pAnimClip->clipLength; // correct time, to it in [0; fAnimEnd]


			size_t frameIndex = static_cast<size_t>(m_currentTime * pAnimClip->keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
			float delta = correctedTime - static_cast<float>(frameIndex); // distance to next frame


			const auto& rotation0 = GetRotationDiscrete(boneIndex, frameIndex);
			const auto& rotation1 =
				(correctedTime >= pAnimClip->clipLength) ? GetRotationDiscrete(frameIndex, frameIndex) : GetRotationDiscrete(boneIndex, 0);


			return sm::Quaternion::Slerp(rotation0, rotation1, delta);
		}
	};


	//class AnimTrackReader_OLD
	//{
	//	const anim_file::AnimFile* m_animFile;
	//	float m_currentTime = 0.0f;

	//public:
	//	AnimTrackReader() = delete;
	//	AnimTrackReader(const anim_file::AnimFile& animFile) : m_animFile(&animFile) {}

	//	float SetTime(float time) { m_currentTime = time; }

	//	sm::Vector3 GetTranslationDiscrete(size_t boneIndex, size_t frmaeIndex) const
	//	{
	//		return m_animFile->frames.at(frmaeIndex).translations.at(boneIndex);
	//	}

	//	sm::Quaternion GetRotationDiscrete(size_t boneIndex, size_t frmaeIndex) const
	//	{
	//		return m_animFile->frames.at(frmaeIndex).rotations.at(boneIndex);
	//	}

	//	sm::Vector3 GetTranslation(size_t boneIndex) const
	//	{
	//		size_t loopCount = static_cast<size_t>(m_currentTime / m_animFile->fileHeader.fEndTime); // number of time that anim could have tun
	//		float correctedTime = m_currentTime - loopCount * m_animFile->fileHeader.fEndTime; // correct time, to it in [0; fAnimEnd]

	//		size_t frameIndex = static_cast<size_t>(correctedTime * m_animFile->fileHeader.fFrameRate); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
	//		float delta = correctedTime - static_cast<float>(frameIndex); // distance to next frame

	//		const auto& translation0 = GetTranslationDiscrete(boneIndex, frameIndex);
	//		const auto& translation1 = (correctedTime >= m_animFile->fileHeader.fEndTime) ?
	//			GetTranslationDiscrete(0, frameIndex) : GetTranslationDiscrete(0, frameIndex);

	//		return sm::Vector3::Lerp(translation1, translation1, delta);
	//	}

	//	sm::Quaternion GetRotation(size_t boneIndex) const
	//	{
	//		size_t loopCount = static_cast<size_t>(m_currentTime / m_animFile->fileHeader.fEndTime); // number of time that anim could have tun
	//		float correctedTime = m_currentTime - loopCount * m_animFile->fileHeader.fEndTime; // correct time, to it in [0; fAnimEnd]


	//		size_t frameIndex = static_cast<size_t>(m_currentTime * m_animFile->fileHeader.fFrameRate); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
	//		float delta = correctedTime - static_cast<float>(frameIndex); // distance to next frame


	//		const auto& rotation0 = GetRotationDiscrete(boneIndex, frameIndex);
	//		const auto& rotation1 = (correctedTime >= m_animFile->fileHeader.fEndTime) ? GetRotationDiscrete(frameIndex, frameIndex) : GetRotationDiscrete(0, frameIndex);


	//		return sm::Quaternion::Slerp(rotation0, rotation1, delta);
	//	}
	//};

};