#pragma once

#include "..\..\Interfaces\TIdentifiable.h"
#include "..\..\Logging\Logging.h"

namespace rldx {

	enum class ResourceTypeEnum
	{
		MeshData,
		ShaderProgram,
		Texture,
		Material,
		BytesStream,
		Skeleton,
		Animation
	};

	class IDxResource : public TIdentifiable<ResourceTypeEnum> {};
}; // namespce rldx