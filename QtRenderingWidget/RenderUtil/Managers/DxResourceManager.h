#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Helpers/NoCaseMapCompare.h"

namespace Rldx {

	class DxTextureView {};
	class DxMesh {};
	class DxTexture {};
	class DxMaterial {};
	class IDxShaderProgram;

	// Id width is set to 32 bit
	using ResId = uint32_t;

	// non-template parent, SHOULD? make sure that the static id is unique per instantiation?
	class ResourceManagerBase
	{
	public:
		virtual ~ResourceManagerBase() = default;
		ResourceManagerBase() = default;
	protected:
		ResId GetNextId() const { return sm_nextId++; }

	private:
		static ResId sm_nextId;
		/*protected:
			static std::unique_ptr<ResourceManagerBase> sm_spoInstance;*/
	};

	template <typename RESOURCE_TYPE>
	class TResourceManager : public ResourceManagerBase
	{
	public:
		RESOURCE_TYPE* GetResourceByString(const std::string& id) const
		{
			auto it = m_resourceDataByString.find(id);
			if (it != m_resourceDataByString.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		RESOURCE_TYPE* GetResourceById(ResId id) const
		{
			auto it = m_resourceDataById.find(id);
			if (it != m_resourceDataById.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		ResId AddResource(std::unique_ptr<RESOURCE_TYPE>& resource, const std::string& stringId = "")
		{
			auto resourceId = GetNextId();
			auto& allocatedResource = m_resourceDataById[resourceId] = std::move(resource);

			// if string supplied associate string with raw pointer to resource
			if (!stringId.empty()) {
				m_resourceDataByString[stringId] = allocatedResource.get(); //m_resourceDataById[resourceId].get();
			}

			return resourceId;
		}

	private:
		std::unordered_map<ResId, std::unique_ptr<RESOURCE_TYPE>> m_resourceDataById;
		std::unordered_map<std::string, RESOURCE_TYPE*, CaseInsensitiveUnorderedMapComparer> m_resourceDataByString;
	};

	class ResourceManager : public ResourceManagerBase
	{
		ResourceManager() {};

	public:
		static ResourceManager& Get();

		TResourceManager<DxMesh>& GetMeshes() { return m_spoMeshManager; }
		TResourceManager<DxTextureView>& GetTextures() { return m_spotextureManager; }
		TResourceManager<DxMaterial>& GetMaterialManager() { return m_spoMaterialManager; }

	private:
		TResourceManager<DxMesh> m_spoMeshManager;
		TResourceManager<DxTextureView> m_spotextureManager;
		TResourceManager<DxMaterial> m_spoMaterialManager;

	private:
		static std::unique_ptr<ResourceManager> sm_spoInstance;
	};

}; // namespace Rldx