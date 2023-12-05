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

	// forward declarations
	class DxTexture;
	class DxMeshData;	
	class DxTexture;
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
	};

	template <typename RESOURCE_TYPE>
	class TResourceManager : public ResourceManagerBase
	{
	public:

		RESOURCE_TYPE* GetResourceById(uint32_t id) const
		{
			auto it = m_resourceDataById.find(id);
			if (it != m_resourceDataById.end())
			{
				return it->second.get();
			}

			return nullptr;
		}

		ResourceHandle<RESOURCE_TYPE> AddResource(const RESOURCE_TYPE* resource, const std::string& stringId = "")
		{
			auto resourceId = GetNextId();
			auto& allocatedResource = m_resourceDataById[resourceId] = std::unique_ptr<RESOURCE_TYPE>(resource, [](RESOURCE_TYPE* p) {delete p; });

			// if string supplied, associate string with raw pointer to resource
			if (!stringId.empty()) {
				m_resourceDataByString[stringId] = allocatedResource.get(); //m_resourceDataById[resourceId].get();			
			}

			return { resourceId, allocatedResource.get() };
		}

		ResourceHandle<RESOURCE_TYPE> AddResource(const RESOURCE_TYPE& resource, const std::string& stringId = "")
		{
			auto resourceId = GetNextId();
			auto& allocatedResource = m_resourceDataById[resourceId] = std::unique_ptr<RESOURCE_TYPE>(resource, [](RESOURCE_TYPE* p) {delete p; });

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
		TResourceManager<DxTexture>& GetTextures() { return m_spoTextureManager; }
		TResourceManager<DxMaterial>& GetMaterialManager() { return m_spoMaterialManager; }
		TResourceManager<IDxShaderProgram>& GetShaderManager() { return m_spoShaderManager; }

	private:
		TResourceManager<DxMeshData> m_spoMeshManager;
		TResourceManager<DxTexture> m_spoTextureManager;
		TResourceManager<DxMaterial> m_spoMaterialManager;
		TResourceManager<IDxShaderProgram> m_spoShaderManager;

	private:
		static std::unique_ptr<DxResourceManager> sm_spoInstance;
	};

}; // namespace Rldx