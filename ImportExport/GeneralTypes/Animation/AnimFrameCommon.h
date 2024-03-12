#pragma once

#include <DirectXPackedVector.h>
#include <SimpleMath.h>
#include <vector>

#include <GeneralTypes\IDataStructure.h>

struct AnimFrameCommon : IDataStructure
{
	AnimFrameCommon(size_t boncCount)
		:
		translations(boncCount),
		rotations(boncCount) {}


	bool IsContentValid() override
	{
		return !translations.empty() && !rotations.empty();
	}

	AnimFrameCommon() = default;
	AnimFrameCommon(size_t translationCount, size_t rotationCount) : translations(translationCount), rotations(rotationCount)
	{}

	std::vector<sm::Vector3> translations;
	std::vector<sm::Quaternion> rotations;
};

using CommonFrames = std::vector<AnimFrameCommon>;