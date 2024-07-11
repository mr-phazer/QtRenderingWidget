#pragma once

#include "..\..\Interfaces\TIdentifiable.h"
#include "Logger\Logger.h"

namespace rldx {

	enum class ResourceTypeEnum
	{
		MeshRenderData,
		ShaderProgram,
		Texture,
		Material,
		BytesStream,
		Skeleton,
		Animation
	};

	class IDxResource : public TIdentifiable<ResourceTypeEnum>
	{
	public:
		IDxResource() = default;
		IDxResource(ResourceTypeEnum type, const std::wstring& typeString, const std::wstring& name)
		{
			SetType(type);
			SetTypeString(typeString);
			SetName(name);
		}
	};
}; // namespce rldx