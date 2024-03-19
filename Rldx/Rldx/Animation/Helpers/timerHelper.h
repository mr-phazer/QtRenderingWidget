#pragma once

namespace skel_anim
{
	class AnimationTimerHelper
	{
		struct TimeValues { float loopedTime; size_t frameIndex0; size_t frameIndex1; float delta; };

		static TimeValues GetTimeValues(float time, float clipLength, float keysPerSecond)
		{
			size_t loopCount = static_cast<size_t>(time / clipLength); // number of times that anim could have tun
			float correctedTime = time - loopCount * clipLength; // correct time, to [0; fAnimEnd] ("loop anim")

			size_t frameIndex0 = static_cast<size_t>(correctedTime * keysPerSecond); // Frame index from  FPS*Time, UNITs:   [second] * [frames]/[second] = [frames]
			float delta = correctedTime - static_cast<float>(frameIndex0); // distance (in unit [frames]) to next frame

			size_t frameCount = static_cast<size_t>(clipLength * keysPerSecond);
			size_t frameIndex1 = (frameIndex0 + 1 >= frameCount) ? 0 : frameIndex0 + 1;

			return { correctedTime, frameIndex0, frameIndex1, delta };
		}
	};
}
