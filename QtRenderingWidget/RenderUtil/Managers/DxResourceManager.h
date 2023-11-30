#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "Helpers/NoCaseMapCompare.h"

namespace Rldx {

	template <typename RESOURCE_TYPE>
	class ResourceHandle
	{
	public:
		ResourceHandle(uint32_t id,  RESOURCE_TYPE* ptr) : m_id(id), m_ptr(ptr) {};

		uint32_t GetId() const { return m_id; };
		RESOURCE_TYPE* GetPtr() const { return m_ptr; };

	private:
		uint32_t m_id;
		RESOURCE_TYPE* m_ptr;
	};

	class DxTextureView {};
	class DxMesh;
	class DxMeshData;
	class DxTexture {};
	class DxMaterial;
	class IDxShaderProgram;
		
	// non-template parent, SHOULD? make sure that the static id is unique per instantiation?
	class ResourceManagerBase
	{
	public:
		virtual ~ResourceManagerBase() = default;
		ResourceManagerBase() = default;
	protected:
		uint32_t GetNextId() const { return sm_nextId++; }

	private:
		static uint32_t sm_nextId;
		/*protected:
			static std::unique_ptr<ResourceManagerBase> sm_spoInstance;*/
	};

	template <typename RESOURCE_TYPE>
	class TResourceManager : public ResourceManagerBase
	{
	public:
	/*	RESOURCE_TYPE* GetResourceByString(const std::string& id) const
		{
			auto it = m_resourceDataByString.find(id);
			if (it != m_resourceDataByString.end())
			{
				return it->second.get();
			}

			return nullptr;
		}*/

		RESOURCE_TYPE* GetResourceById(uint32_t id) const
		{
			auto it = m_resourceDataById.find(id);
			if (it != m_resourceDataById.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		ResourceHandle<RESOURCE_TYPE> AddResource(const RESOURCE_TYPE& resource, const std::string& stringId = "")
		{
			auto resourceId = GetNextId();
			auto& allocatedResource = m_resourceDataById[resourceId] = std::make_unique<RESOURCE_TYPE>(resource);

			// if string supplied, associate string with raw pointer to resource
			if (!stringId.empty()) {
				m_resourceDataByString[stringId] = allocatedResource.get(); //m_resourceDataById[resourceId].get();			
			}

			return {resourceId, allocatedResource.get()};
		}

	private:
		std::unordered_map<uint32_t, std::unique_ptr<RESOURCE_TYPE>> m_resourceDataById;

		std::unordered_map<
			std::string, 
			RESOURCE_TYPE*, 
			CaseInsensitiveUnorderedMapComparer::Hash,
			CaseInsensitiveUnorderedMapComparer::Compare> m_resourceDataByString;
	};

	class DxResourceManager : public ResourceManagerBase
	{
		DxResourceManager() {};

	public:
		static DxResourceManager& GetInstance();

		TResourceManager<DxMeshData>& GetMeshes() { return m_spoMeshManager; }
		TResourceManager<DxTextureView>& GetTextures() { return m_spotextureManager; }
		TResourceManager<DxMaterial>& GetMaterialManager() { return m_spoMaterialManager; }

	private:
		TResourceManager<DxMeshData> m_spoMeshManager;
		TResourceManager<DxTextureView> m_spotextureManager;
		TResourceManager<DxMaterial> m_spoMaterialManager;

	private:
		static std::unique_ptr<DxResourceManager> sm_spoInstance;
	};

}; // namespace Rldx