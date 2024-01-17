#pragma once

#include "..\rldx\Interfaces\TIdentifiable.h"

namespace rldx {

	enum class ResourceTypeEnum
	{
		MeshData,
		ShaderProgram,
		Texture,
		Material
	};

	class IDxResource : public TIdentifiable<ResourceTypeEnum>
	{
		
	
	};
}; // namespce rldx