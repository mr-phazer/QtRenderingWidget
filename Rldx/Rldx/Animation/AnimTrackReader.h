#pragma once

#include <FileFormats\Anim\Types\Common\TwAnimFile.h>
#include <SimpleMath.h>
#include <vector>
#include "DataTypes\SkeletonAnimation.h"


namespace skel_anim
{
	class AnimTrackReader
	{
		const SkeletonAnimation* pAnimClip = nullptr;

	public:
		AnimTrackReader() = default;

		void SetAnimClip(const SkeletonAnimation* animClip) { pAnimClip = animClip; };

		sm::Vector3 GetTranslationDiscrete(size_t boneIndex, size_t frameIndex) const
		{
			if (frameIndex >= pAnimClip->frameData.frames.size())
			{
				return pAnimClip->frameData.frames.at(0).boneKeys.at(boneIndex).translation;
			}

			return pAnimClip->frameData.frames.at(frameIndex).boneKeys.at(boneIndex).translation;
		}

		sm::Quaternion GetRotationDiscrete(size_t boneIndex, size_t frameIndex) const
		{
			if (frameIndex >= pAnimClip->frameData.frames.size())
			{
				return pAnimClip->frameData.frames.at(0).boneKeys.at(boneIndex).rotation;
			}

			return pAnimClip->frameData.frames.at(frameIndex).boneKeys.at(boneIndex).rotation;
		}

		struct { float loopedTime; size_t frameIndex; float delta; } GetTimeValues(float time) const
		{
			size_t loopCount = static_cast<size_t>(time / pAnimClip->lastKeyTime); // number of time that anim could have tun
			float correctedTime = time - loopCount * pAnimClip->lastKeyTime; // correct time, to it in [0; fAnimEnd]

			size_t frameIndex = static_cast<size_t>(correctedTime * pAnimClip->keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
			float delta = correctedTime * pAnimClip->keysPerSecond - frameIndex; // distance to next frame

			return { correctedTime, frameIndex, delta };
		}

		sm::Vector3 GetTranslation(size_t boneIndex, float time) const
		{
			auto timeValues = GetTimeValues(time);

			const auto& translation0 = GetTranslationDiscrete(boneIndex, timeValues.frameIndex);
			const auto& translation1 = GetTranslationDiscrete(boneIndex, timeValues.frameIndex + 1);

			return sm::Vector3::Lerp(translation0, translation1, timeValues.delta);
		}

		sm::Quaternion GetRotation(size_t boneIndex, float time) const
		{
			//size_t loopCount = static_cast<size_t>(time / pAnimClip->lastKeyTime); // number of time that anim could have tun
			//float correctedTime = time - loopCount * pAnimClip->lastKeyTime; // correct time, to it in [0; fAnimEnd]


			//size_t frameIndex = static_cast<size_t>(correctedTime * pAnimClip->keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
			//float interpolationFactor = correctedTime - static_cast<float>(frameIndex); // distance to next frame

			//// if the time is beyond the last key?
			//auto testTime = pAnimClip->lastKeyTime - frameIndex * (1.0f / pAnimClip->keysPerSecond);


			auto timeValues = GetTimeValues(time);

			const auto& rotation0 = GetRotationDiscrete(boneIndex, timeValues.frameIndex);
			const auto& rotation1 = GetRotationDiscrete(boneIndex, timeValues.frameIndex + 1);

			return sm::Quaternion::Slerp(rotation0, rotation1, timeValues.delta);
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

	//	sm::Vector3 GetTranslationDiscrete(size_t boneIndex, size_t frameIndex) const
	//	{
	//		return m_animFile->frameData.frames.at(frameIndex).translations.at(boneIndex);
	//	}

	//	sm::Quaternion GetRotationDiscrete(size_t boneIndex, size_t frameIndex) const
	//	{
	//		return m_animFile->frameData.frames.at(frameIndex).rotations.at(boneIndex);
	//	}

	//	sm::Vector3 GetTranslation(size_t boneIndex) const
	//	{
	//		size_t loopCount = static_cast<size_t>(time / m_animFile->fileHeader.fLastKeyTime); // number of time that anim could have tun
	//		float correctedTime = time - loopCount * m_animFile->fileHeader.fLastKeyTime; // correct time, to it in [0; fAnimEnd]

	//		size_t frameIndex = static_cast<size_t>(correctedTime * m_animFile->fileHeader.fFrameRate); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
	//		float interpolationFactor = correctedTime - static_cast<float>(frameIndex); // distance to next frame

	//		const auto& translation0 = GetTranslationDiscrete(boneIndex, frameIndex);
	//		const auto& translation1 = (correctedTime >= m_animFile->fileHeader.fLastKeyTime) ?
	//			GetTranslationDiscrete(0, frameIndex) : GetTranslationDiscrete(0, frameIndex);

	//		return sm::Vector3::Lerp(translation1, translation1, interpolationFactor);
	//	}

	//	sm::Quaternion GetRotation(size_t boneIndex) const
	//	{
	//		size_t loopCount = static_cast<size_t>(time / m_animFile->fileHeader.fLastKeyTime); // number of time that anim could have tun
	//		float correctedTime = time - loopCount * m_animFile->fileHeader.fLastKeyTime; // correct time, to it in [0; fAnimEnd]


	//		size_t frameIndex = static_cast<size_t>(time * m_animFile->fileHeader.fFrameRate); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
	//		float interpolationFactor = correctedTime - static_cast<float>(frameIndex); // distance to next frame


	//		const auto& rotation0 = GetRotationDiscrete(boneIndex, frameIndex);
	//		const auto& rotation1 = (correctedTime >= m_animFile->fileHeader.fLastKeyTime) ? GetRotationDiscrete(frameIndex, frameIndex) : GetRotationDiscrete(0, frameIndex);


	//		return sm::Quaternion::Slerp(rotation0, rotation1, interpolationFactor);
	//	}
	//};

};