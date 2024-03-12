#pragma once

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <SimpleMath.h>
#include <vector>
#include "Skeleton.h"


namespace skel_anim
{
	class AnimTrackReader
	{
		const SkeletonAnimationClip* pAnimClip = nullptr;

	public:
		AnimTrackReader() = default;

		void SetAnimClip(const SkeletonAnimationClip* animClip) { pAnimClip = animClip; };

		sm::Vector3 GetTranslationDiscrete(size_t boneIndex, size_t frmaeIndex) const
		{
			return pAnimClip->frames.at(frmaeIndex).boneKeys.at(boneIndex).translation;
		}

		sm::Quaternion GetRotationDiscrete(size_t boneIndex, size_t frmaeIndex) const
		{
			return pAnimClip->frames.at(frmaeIndex).boneKeys.at(boneIndex).rotation;
		}

		sm::Vector3 GetTranslation(size_t boneIndex, float time = 0.0f) const
		{
			size_t loopCount = static_cast<size_t>(time / pAnimClip->clipLength); // number of time that anim could have tun
			float correctedTime = time - loopCount * pAnimClip->clipLength; // correct time, to it in [0; fAnimEnd]

			size_t frameIndex = static_cast<size_t>(correctedTime * pAnimClip->keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
			float delta = correctedTime - static_cast<float>(frameIndex); // distance to next frame

			const auto& translation0 = GetTranslationDiscrete(boneIndex, frameIndex);
			const auto& translation1 = (correctedTime >= pAnimClip->clipLength) ?
				GetTranslationDiscrete(boneIndex, 0) : GetTranslationDiscrete(boneIndex, frameIndex);

			return sm::Vector3::Lerp(translation0, translation1, delta);
		}

		sm::Quaternion GetRotation(size_t boneIndex, float time = 0.0f) const
		{
			size_t loopCount = static_cast<size_t>(time / pAnimClip->clipLength); // number of time that anim could have tun
			float correctedTime = time - loopCount * pAnimClip->clipLength; // correct time, to it in [0; fAnimEnd]


			size_t frameIndex = static_cast<size_t>(time * pAnimClip->keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
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
	//	float time = 0.0f;

	//public:
	//	AnimTrackReader() = delete;
	//	AnimTrackReader(const anim_file::AnimFile& animFile) : m_animFile(&animFile) {}

	//	float SetTime(float time) { time = time; }

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
	//		size_t loopCount = static_cast<size_t>(time / m_animFile->fileHeader.fEndTime); // number of time that anim could have tun
	//		float correctedTime = time - loopCount * m_animFile->fileHeader.fEndTime; // correct time, to it in [0; fAnimEnd]

	//		size_t frameIndex = static_cast<size_t>(correctedTime * m_animFile->fileHeader.fFrameRate); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
	//		float delta = correctedTime - static_cast<float>(frameIndex); // distance to next frame

	//		const auto& translation0 = GetTranslationDiscrete(boneIndex, frameIndex);
	//		const auto& translation1 = (correctedTime >= m_animFile->fileHeader.fEndTime) ?
	//			GetTranslationDiscrete(0, frameIndex) : GetTranslationDiscrete(0, frameIndex);

	//		return sm::Vector3::Lerp(translation1, translation1, delta);
	//	}

	//	sm::Quaternion GetRotation(size_t boneIndex) const
	//	{
	//		size_t loopCount = static_cast<size_t>(time / m_animFile->fileHeader.fEndTime); // number of time that anim could have tun
	//		float correctedTime = time - loopCount * m_animFile->fileHeader.fEndTime; // correct time, to it in [0; fAnimEnd]


	//		size_t frameIndex = static_cast<size_t>(time * m_animFile->fileHeader.fFrameRate); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
	//		float delta = correctedTime - static_cast<float>(frameIndex); // distance to next frame


	//		const auto& rotation0 = GetRotationDiscrete(boneIndex, frameIndex);
	//		const auto& rotation1 = (correctedTime >= m_animFile->fileHeader.fEndTime) ? GetRotationDiscrete(frameIndex, frameIndex) : GetRotationDiscrete(0, frameIndex);


	//		return sm::Quaternion::Slerp(rotation0, rotation1, delta);
	//	}
	//};

};