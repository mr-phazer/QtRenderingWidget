#pragma once

#include "..\Interfaces\IIdentifiable.h"

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