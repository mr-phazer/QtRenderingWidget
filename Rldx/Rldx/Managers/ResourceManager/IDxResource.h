#pragma once

#include "..\..\Interfaces\TIdentifiable.h"
#include "Logger\Logger.h"

namespace rldx {

	enum class ResourceTypeEnum
	{
		NOT_SET,
		Mesh,
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
		IDxResource()
		{
			SetType(ResourceTypeEnum::NOT_SET);
			SetTypeString(L"IDxResouce:Base");
			SetName(L"Unnamed");
		}

		IDxResource(ResourceTypeEnum type, const std::wstring& typeString, const std::wstring& name)
		{
			SetType(type);
			SetTypeString(typeString);
			SetName(name);
		}
	};
}; // namespce rldx