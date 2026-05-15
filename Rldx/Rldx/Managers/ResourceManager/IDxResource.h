#pragma once

#include <filesystem>

#include "..\..\Interfaces\TIdentifiable.h"
#include <CommonLibs/Logger/Logger.h>

namespace fs = std::filesystem;

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
	//protected:
		IDxResource(const std::wstring& name)
		{
			SetType(ResourceTypeEnum::NOT_SET);
			SetTypeString(L"IDxResouce:Base");
			SetName(name == L"" ? GetIdString() : name);
		}

		/*IDxResource(ResourceTypeEnum type, const std::wstring& typeString, const std::wstring& name)
		{
			SetType(type);
			SetTypeString(typeString);
			SetName(name);
		}		*/
	};
}; // namespce rldx