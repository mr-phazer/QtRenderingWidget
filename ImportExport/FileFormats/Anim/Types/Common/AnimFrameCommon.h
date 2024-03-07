#pragma once

#include <DirectXPackedVector.h>
#include <SimpleMath.h>
#include <vector>

namespace anim_file {

	struct AnimFrameCommon
	{
		AnimFrameCommon() = default;
		AnimFrameCommon(size_t translationCount, size_t rotationCount) : translations(translationCount), rotations(rotationCount)
		{}

		std::vector<sm::Vector3> translations;
		std::vector<sm::Quaternion> rotations;
	};
}