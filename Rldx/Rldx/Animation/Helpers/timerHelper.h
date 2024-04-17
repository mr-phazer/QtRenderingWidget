#pragma once

namespace skel_anim
{
	class AnimationTimerHelper
	{
	public:
		struct TimeValues { float loopedTime = 0.0f; size_t frameIndex0 = 0; size_t frameIndex1 = 0; float interpolationFactor = 0.0f; };

		static TimeValues GetLoopedTimeValues(float time, float clipLength, float keysPerSecond)
		{
			float correctedTime = time < 0 ? clipLength - time : time; // if time is negative, correct it

			size_t loopCount = static_cast<size_t>(correctedTime / clipLength); // number of times that anim could have tun
			float loopedTime = correctedTime - loopCount * clipLength; // correct correctedTime, to [0; fAnimEnd] ("loop anim")

			size_t frameIndex0 = static_cast<size_t>(loopedTime * keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
			float delta = loopedTime * keysPerSecond - static_cast<float>(frameIndex0); // distance (in unit [frames]) to next frame

			size_t frameCount = static_cast<size_t>(clipLength * keysPerSecond);
			size_t frameIndex1 = (frameIndex0 + 1 >= frameCount) ? 0 : frameIndex0 + 1;

			return { loopedTime, frameIndex0, frameIndex1, delta };
		}
	};
}
