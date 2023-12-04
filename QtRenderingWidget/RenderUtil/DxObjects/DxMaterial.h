#pragma once

#include <vector>

namespace Rldx {
	
	class DxTexture;
	
	struct ShaderInput {
		uint32_t channel, DxTexture;
	};

	class DxMaterial
	{
		std::vector<ShaderInput> textures;
	};
}

