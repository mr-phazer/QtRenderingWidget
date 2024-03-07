#pragma once

#include <DirectXPackedVector.h>
#include <SimpleMath.h>
#include <vector>

namespace anim_file {

	struct AnimFrameRaw
	{
		std::vector <sm::Vector3> translations;
		std::vector<DirectX::PackedVector::XMUSHORT4> rotations;
	};
}