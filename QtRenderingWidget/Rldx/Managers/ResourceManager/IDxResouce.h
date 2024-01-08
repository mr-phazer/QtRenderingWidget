#pragma once

#include "..\RlDx\Interfaces\TIdentifiable.h"

namespace Rldx {

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
}; // namespce Rldx