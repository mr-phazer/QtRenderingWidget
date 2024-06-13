#pragma once

#include "..\..\Interfaces\TIdentifiable.h"
#include "Logger\Logger.h"

namespace rldx {

	enum class ResourceTypeEnum
	{
		MeshData,
		ShaderProgram,
		Texture,
		Material,
		BytesStream,
		Animation
	};

	class IDxResource : public TIdentifiable<ResourceTypeEnum> {};
}; // namespce rldx